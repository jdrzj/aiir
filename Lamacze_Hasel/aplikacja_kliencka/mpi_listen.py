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
      if result.get('password') is None:
        password.status = F('status') + 1

      else:
        password.password = result['password']

        password.start_time = datetime.fromtimestamp(int(result['start_time']))
        password.end_time = datetime.fromtimestamp(int(result['end_time']))
      
      password.save()
      print('%s received %s' % (self.cluster_id, result))