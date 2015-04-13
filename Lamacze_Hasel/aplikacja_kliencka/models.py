from django.db import models
from django.contrib.auth.models import User
import json

class Task(models.Model):
	user = models.ForeignKey(User)
	status = models.IntegerField()
	cluster = models.CharField(max_length=250)
	creation_date = models.DateTimeField(auto_now_add=True)
	end_time = models.DateTimeField(null=True, blank=True)

	def __str__(self):
		return self.cluster

	def json(self):
		return dict(
			id = self.id,
			status = self.status,
			creation_date = str(self.creation_date),
			cluster = self.cluster,
			passwords = [ password.json(True) for password in self.passwords.all()])

class Password(models.Model):

	CRACKING_ALGORITHMS = (
    	('MS', 'Metoda Słownikowa'),
    	('TT', 'Tablice Tęczowe'),
    	('BF', 'Brute Force'),
	)
	hash = models.CharField(max_length=250)
	password = models.CharField(max_length=250)
	task = models.ForeignKey(Task, related_name='passwords')
	start_time = models.DateTimeField(null=True)
	end_time = models.DateTimeField(null=True)
	algorithm = models.CharField(max_length=30)
	password_cracking_algorithm = models.CharField(max_length=2, choices=CRACKING_ALGORITHMS)

	def json(self, full):
		if full:
			return dict(
				id = self.id,
				hash = self.hash,
				start_time = str(self.start_time),
				end_time = str(self.end_time),
				algorithm = self.algorithm,
				password = self.password)
		else:
			return json.dumps(dict(
				id = str(self.id),
				hash = self.hash,
				algorithm = self.algorithm))
