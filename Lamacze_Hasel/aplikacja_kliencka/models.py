from django.db import models
from django.contrib.auth.models import User

class Task(models.Model):
	user = models.ForeignKey(User)
	status = models.IntegerField()
	cluster = models.CharField(max_length=250)
	creation_date = models.DateTimeField(auto_now_add=True)

	def json(self):
		return dict(
			id = self.id,
			status = self.status,
			creation_date = str(self.creation_date),
			cluster = self.cluster,
			passwords = [ password.json() for password in self.passwords.all()])

class Password(models.Model):
	hash = models.CharField(max_length=250)
	password = models.CharField(max_length=250)
	task = models.ForeignKey(Task, related_name='passwords')
	start_time = models.DateTimeField(null=True)
	end_time = models.DateTimeField(null=True)
	algorithm = models.CharField(max_length=30)

	def json(self):
		return dict(
			id = self.id,
			hash = self.hash,
			start_time = str(self.start_time),
			end_time = str(self.end_time),
			algorithm = self.algorithm,
			password = self.password)
