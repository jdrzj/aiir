from aplikacja_kliencka.models import *
from threading import Thread
from datetime import datetime
from django.db.models import F
import pika
import json

connection = pika.BlockingConnection(pika.ConnectionParameters(host='node1'))
channel = connection.channel()
channel.exchange_declare(exchange='mpi.results', type='direct')

channel.basic_qos(prefetch_count=1)
result = channel.queue_declare(exclusive=True)
queue_name = result.method.queue
channel.queue_bind(exchange='mpi.results', queue=queue_name, routing_key='mpi.results')

class GetMessage(Thread):
	def run(self):
		def callback(ch, method, properties, body):
			result = json.loads(body.decode('utf-8'))
			password = Password.objects.filter(id=result['id'])[0]
			password.password = result['password']
			password.end_time = datetime.now()
			password.save()

			print('Received')
			print(result)
		
		channel.basic_consume(callback, queue=queue_name, no_ack=True)
		channel.start_consuming()

t = GetMessage()
t.start()
