from django.shortcuts import HttpResponse
from django.contrib.auth.models import User
from django.views.decorators.csrf import csrf_exempt
from aplikacja_kliencka.models import *
import json
import pika


connection = pika.BlockingConnection(pika.ConnectionParameters('node1'))
channel = connection.channel()	
channel.exchange_declare(exchange='mpi.tasks', type='direct')

@csrf_exempt
def send(request):
	if request.method == 'POST':
		reqData = json.loads(request.body.decode('utf-8'))

		task = Task(status=0, user=request.user, cluster=reqData['cluster'])
		task.save()
		passwords = [Password(hash=h['hash'], algorithm=h['algorithm'], task=task) for h in reqData['hashes']]
		for password in passwords:
			password.save()
		channel.basic_publish(exchange='mpi.tasks', routing_key=task.cluster, body=json.dumps(task.json()['passwords']))

		return HttpResponse(json.dumps(task.json()), content_type='application/json')
	return None
