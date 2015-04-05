from django.shortcuts import HttpResponse
from django.contrib.auth.models import User
from django.views.decorators.csrf import csrf_exempt
from django.db.models import F
from django.conf import settings
from aplikacja_kliencka.models import *
from aplikacja_kliencka import mpi_utils, mpi_listen
import json, zmq, time

context = zmq.Context()
taskQ = context.socket(zmq.PUB)
taskQ.bind('tcp://*:5557')

mpi_utils.start_deamons(settings.CLUSTERS, settings.MY_SOCKET_ADDRESS)
time.sleep(3)

for cluster in settings.CLUSTERS:
  receiver = mpi_utils.sync_cluster(cluster, taskQ, context);
  t = mpi_listen.GetMessages(receiver, cluster['id'])
  t.start()

@csrf_exempt
def send(request):
  if request.method == 'POST':
    reqData = json.loads(request.body.decode('utf-8'))

    task = Task(status=0, user=request.user, cluster=reqData['cluster'])
    task.save()
    passwords = [Password(hash=h['hash'], algorithm=h['algorithm'], task=task) for h in reqData['hashes']]
    for password in passwords:
      password.save()
      message = "%s %s" % (task.cluster, password.json(False))
      taskQ.send_string(message)

    return HttpResponse(json.dumps(task.json()), content_type='application/json')
  return None