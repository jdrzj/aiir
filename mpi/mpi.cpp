#include "zhelpers.hpp"
#include "task.hpp"
#include "Attack.h"

#include <mpi.h>

void master(char* django, char* clusterId, int world_size, int seed);
void slave(int world_rank);
void syncRecv(zmq::socket_t* recv);
void syncTransmiter(zmq::socket_t* transmiter);
bool recvMessage(int source, int tag, int *realSource, Subtask& subtask, std::string& result, int world_rank, bool allow_struct);
char getMethod(std::string message);
char getHashingFunction(std::string message);
std::string getHash(std::string message);

///////////////////////////////////////////////////////////////////////////////

const int nitems = 3;
int blocklengths[3] = { 1, 1, 1 };
MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};
long int offsets[3] { 0, 4, 8 };
MPI_Datatype MPI_Subtask_type;

///////////////////////////////////////////////////////////////////////////////

// argv[1] = django sending adress & port
// argv[2] = cluster id
// argv[3] = seed
int main (int argc, char *argv[])
{
    MPI_Init (&argc, &argv);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &MPI_Subtask_type);
    MPI_Type_commit(&MPI_Subtask_type);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size < 2) {
        std::cerr << "World size must be greater than 1" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int number;
    if (world_rank == 0) {
        master(argv[1], argv[2], world_size, std::stoi(argv[3]));
    } else {
        slave(world_rank);
    }

    MPI_Type_free(&MPI_Subtask_type);
    MPI_Finalize();
}

void master(char* django, char* clusterId, int world_size, int seed)
{
    std::cout << "Master" << std::endl;
    zmq::context_t context(1);

    //  Socket to receive messages on
    zmq::socket_t taskQ(context, ZMQ_SUB);
    taskQ.connect(django);
    taskQ.setsockopt( ZMQ_SUBSCRIBE, clusterId, 1);

    //  Socket to send messages to
    zmq::socket_t resultQ(context, ZMQ_PUB);
    resultQ.bind("tcp://*:5558");

    syncRecv(&taskQ);
    syncTransmiter(&resultQ);


    Task* task;
    while (true)
    {
        zmq::message_t message;
        taskQ.recv(&message);
        task = new Task(&message);
        SubtaskQueue<Subtask> subtask_queue = task->getSubtaskQueue();
        task->setIntervalCount(subtask_queue.size());
        s_send(resultQ, task->getProgressJson());
        task->start();

        std::string hash = task->getHash();
        int sent = 0;
        for (int i = 1; i < world_size; i++)
        {
            std::cout << "MASTER: Sending hash and method to all the slaves" << std::endl;
            MPI_Send((void*)hash.c_str(), hash.size(), MPI_CHAR, i, 0, MPI_COMM_WORLD);
            ++sent;
        }

        for (int i = 1; i < world_size; i++)
        {
            std::cout << "MASTER: Distributing first subtasks" << std::endl;
            Subtask new_subtask = subtask_queue.front();
            subtask_queue.pop();
            task->incrementProgress();
            s_send(resultQ, task->getProgressJson());
            MPI_Send(&new_subtask, 1, MPI_Subtask_type, i, 0, MPI_COMM_WORLD);
            ++sent;
        }

        std::string recvPass;
        Subtask subtask;
        std::string result;

        std::string password = "N/A";
        do
        {
            int realSource = 0;
            recvMessage(MPI_ANY_SOURCE, 0, &realSource, subtask, result, 0, false);
            --sent;
            std::cout << "MASTER: Received " << result << " from SLAVE no. " << realSource << std::endl;

            if (result.compare(".") != 0) {
                password = result;
                task->setProgress(task->getIntervalCount());
                s_send(resultQ, task->getProgressJson());
                break;
            }
            else if (!subtask_queue.empty())
            {
                ++sent;
                Subtask new_subtask = subtask_queue.front();
                subtask_queue.pop();
                task->incrementProgress();
                MPI_Send(&new_subtask, 1, MPI_Subtask_type, realSource, 0, MPI_COMM_WORLD);
                s_send(resultQ, task->getProgressJson());
            }
            else
                break;
        } while (true);

        task->stop();
        task->setPassword(password);

        s_send(resultQ, task->getJson());

        // Cleaning
        for (int i = 0; i < sent; i++)
        {
            int realSource = 0;
            recvMessage(MPI_ANY_SOURCE, 0, &realSource, subtask, result, 0, false);
        }
        std::cout << "MASTER: Cleaned up after this task.\n";
    }
}

void syncRecv(zmq::socket_t* recv)
{
    Task* task;
    while (true)
    {
        zmq::message_t message;
        recv->recv(&message);

        task = new Task(&message);
        std::string id = task->getId();
        std::cout << "-> " << id << std::endl;

        if (id == "@") {
            break;
        }

        delete task;
    }
}

void syncTransmiter(zmq::socket_t* transmiter)
{
    Task* hashTask = new Task("#");
    Task* atTask = new Task("@");

    for (int i = 0; i < 10; i++) {
        s_send(*transmiter, hashTask->getJson());
        usleep(100);
        std::cout << "<- #" << std::endl;
    }
    s_send(*transmiter, atTask->getJson());
    std::cout << "<- @" << std::endl;

    delete hashTask;
    delete atTask;
}

void slave(int world_rank)
{
    int realSource = 0;
    std::string password;
    Subtask subtask;
    std::string result;
    std::string hash;
    AttackType type;
    HashingFunction hashing_function;
    char method;
    char hashing_function_used;

    while (true)
    {
        // hardkodowe SHA-1
        bool msg_type = recvMessage(0, 0, &realSource, subtask, result, world_rank, true);

        if (msg_type == 0)
        {
            std::cout << "SLAVE #" << world_rank << ": Got a message: " << result << std::endl;
            method = getMethod(result);
            hash = getHash(result);
            switch (method)
            {
                case 'B':
                    type = AttackType::bruteForce;
                    break;
                case 'D':
                    type = AttackType::dictionary;
                    break;
                case 'R':
                    type = AttackType::rainbow;
                    break;
            }
            hashing_function_used = getHashingFunction(result);
            switch (hashing_function_used)
            {
                case 'M':
                    hashing_function = HashingFunction::MD5;
                    break;
                case 'S':
                     hashing_function = HashingFunction::SHA1;
                    break;
            }
        }
        else
        {
            Attack *attack = NULL;
            attack = new Attack(type, hash, hashing_function);
            attack->setLettersCount(subtask.pass_length);
            attack->setChainsRange(subtask.start, subtask.end);
            password = attack->defeatKey();
            if (attack)
                delete attack; 
        }
        if (password == "")
            password = ".";

        MPI_Send((void*)password.c_str(), password.size(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
}

// 0 - wiadomość z hashem,
// 1 - wiadomość z subtaskiem
bool recvMessage(int source, int tag, int *realSource, Subtask& subtask, std::string& result, int world_rank, bool allow_struct)
{
    MPI_Status status;
    MPI_Probe(source, tag, MPI_COMM_WORLD, &status);
    int msg_size;
    MPI_Get_count(&status, MPI_BYTE, &msg_size);

    if (allow_struct && msg_size == 3 * sizeof(int))
    {
        Subtask recv;
        MPI_Recv(&recv, 1, MPI_Subtask_type, source, tag, MPI_COMM_WORLD, &status);
        std::string text = world_rank ? "SLAVE #" + std::to_string(world_rank) : "MASTER";
        std::cout << text << ": Received data (start = " << recv.start << ", end = "
                  << recv.end << ", password length = " << recv.pass_length << ")\n";
        subtask = recv;
        return 1;
    }
    else
    {
        char* recv = (char*)malloc(sizeof(char) * msg_size);
        MPI_Recv(recv, msg_size, MPI_CHAR, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::string new_result(recv, msg_size);
        free(recv);
        *realSource = status.MPI_SOURCE;
        result = new_result;
        return 0;
    }
}

char getMethod(std::string message)
{
    return message[0];
}

char getHashingFunction(std::string message)
{
    return message[1];
}

std::string getHash(std::string message)
{
    return message.substr(2, message.length());
}