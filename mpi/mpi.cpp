#include "zhelpers.hpp"
#include "task.hpp"
#include "Attack.h"
#include <mpi.h>

void master(char* django, char* clusterId, int world_size);
void slave(int world_rank);
void syncRecv(zmq::socket_t* recv);
void syncTransmiter(zmq::socket_t* transmiter);
std::string revcString(int source, int tag);

// argv[1] = django sending adress & port
// argv[2] = cluster id
int main (int argc, char *argv[])
{

    MPI_Init (&argc, &argv);
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
        master(argv[1], argv[2], world_size);
    } else {
        slave(world_rank);
    }

    MPI_Finalize();
}

void master(char* django, char* clusterId, int world_size)
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
    while (true) {
        zmq::message_t message;
        taskQ.recv(&message);
        task = new Task(&message);
        task->start();

        std::string hash = task->getHash();
        std::cout << "Master sending = " << hash << std::endl;
        for (int i = 1; i < world_size; i++) {
            MPI_Send((void*)hash.c_str(), hash.size(), MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
        // MPI_Bcast((void*)hash.c_str(), hash.size(), MPI_CHAR, 0, MPI_COMM_WORLD);

        std::string password = "N/A";
        for (int i = 1; i < world_size; i++) {
            std::string recvPass = revcString(i, 0);
            std::cout << "Master get = " << recvPass << std::endl;

            if (recvPass != "N/A") {
                password = recvPass;
            }
        }
        task->stop();
        task->setPassword(password);

        s_send(resultQ, task->getJson());
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


    while (true) {
        std::string hash = revcString(0, 0);
        Attack *dict = new Attack(AttackType::dictionary, hash, HashingFunction::SHA1);
        
        dict->setDictionaryFileName("/vagrant/john.txt");

        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        dict->defeatKey(world_rank, world_size);

        std::string password;

        password = hash;
        MPI_Send((void*)password.c_str(), password.size(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
}

std::string revcString(int source, int tag)
{
    // http://mpitutorial.com/tutorials/dynamic-receiving-with-mpi-probe-and-mpi-status/
    MPI_Status status;
    int len;
    // Probe for an incoming message from process zero
    MPI_Probe(source, tag, MPI_COMM_WORLD, &status);

    // When probe returns, the status object has the size and other
    // attributes of the incoming message. Get the message size
    MPI_Get_count(&status, MPI_CHAR, &len);
    char* recv = (char*)malloc(sizeof(char) * len);
    MPI_Recv(recv, len, MPI_CHAR, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    std::string result(recv);
    free(recv);

    return result;
}
