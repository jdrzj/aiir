import paramiko, time, json, zmq

hashTask = json.dumps(dict(id="#"))
atTask = json.dumps(dict(id="@"))

def sync_cluster(cluster, transmiter, context):
  for i in range(0, 10):
    transmiter.send_string("%s %s" % (cluster['id'], hashTask))
    time.sleep(0.01)
  transmiter.send_string("%s %s" % (cluster['id'], atTask))

  receiver = context.socket(zmq.SUB)
  receiver.connect(cluster['socket_address'])
  receiver.setsockopt_string(zmq.SUBSCRIBE, '')

  while True:
    message = receiver.recv_string()
    message = json.loads(message)
    print(message['id'])
    if message['id'] == '@':
        break;
  print('Synchronized with %s' % cluster['id'])

  return receiver

def start_deamons(clusters, my_address, seed):
  for cluster in clusters:
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(hostname=cluster['address'],
      username=cluster['username'],
      password=cluster['password'])

    client.exec_command('CLUSTER_ID=%s DJANGO_ADRESS=%s SEED=%s /etc/init.d/mpi restart' % (cluster['id'], my_address, seed))
