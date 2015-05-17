from aplikacja_kliencka.models import *
from django.db.models import F
from threading import Thread
from datetime import datetime
import json

class GetMessages(Thread):
  def __init__(self, receiver, cluster_id):
    Thread.__init__(self)

    self.receiver = receiver
    self.cluster_id = cluster_id

  def run(self):
    while(True):
      result = json.loads(self.receiver.recv_string())
      result['id'] = int(result['id'])

      password = Password.objects.filter(id=result['id'])[0]
      task = Task.objects.filter(id=password.task.id)[0]

      # Dostalismy JSON-a z progressem
      if result.get('password') is None:

        # ustawiamy status hasla
        password.status = int(result['status'])
        password.save()

        print('Got password status: ', password.status)

        # liczymy status calego zadania
        passwords_per_task = task.passwords.all().count()
        print('Passwords per task: ' + str(passwords_per_task))
        status_sum = 0

        for single_password in task.passwords.all():
          print ('Passowrd status: ' + str(single_password.status))
          status_sum += int(single_password.status)

        task.status = int(status_sum / passwords_per_task)

      else:
        password.password = result['password']
        password.start_time = datetime.fromtimestamp(int(result['start_time']))
        password.end_time = datetime.fromtimestamp(int(result['end_time']))

      # Zakonczenie taska
      if task.status is 100:
        task.end_time = password.end_time
      
      task.save()
      password.save()
      print('Cluster %s received %s' % (self.cluster_id, result))
