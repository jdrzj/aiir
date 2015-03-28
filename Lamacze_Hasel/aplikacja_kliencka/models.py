from django.db import models
from django.contrib.auth.models import User

class Task(models.Model):
	user = models.ForeignKey(User)
	status = models.IntegerField()
	cluster = models.CharField(max_length=250)

	def json(self):
		return dict(
			id = self.id,
			status = self.status,
			passwords = [ password.json() for password in self.passwords.all()])

class Password(models.Model):
	hash = models.CharField(max_length=250)
	password = models.CharField(max_length=250)
	task = models.ForeignKey(Task, related_name='passwords')

	def json(self):
		return dict(
			id = self.id,
			hash = self.hash,
			password = self.password)
