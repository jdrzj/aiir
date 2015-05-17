# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
from django.conf import settings


class Migration(migrations.Migration):

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
    ]

    operations = [
        migrations.CreateModel(
            name='Password',
            fields=[
                ('id', models.AutoField(serialize=False, verbose_name='ID', auto_created=True, primary_key=True)),
                ('hash', models.CharField(max_length=250)),
                ('password', models.CharField(null=True, blank=True, max_length=250)),
                ('status', models.IntegerField()),
                ('start_time', models.DateTimeField(null=True, blank=True)),
                ('end_time', models.DateTimeField(null=True, blank=True)),
                ('algorithm', models.CharField(max_length=30)),
                ('password_cracking_algorithm', models.CharField(choices=[('MS', 'Metoda Slownikowa'), ('TT', 'Tablice Teczowe'), ('BF', 'Brute Force')], max_length=2)),
                ('graininess', models.IntegerField()),
            ],
        ),
        migrations.CreateModel(
            name='Task',
            fields=[
                ('id', models.AutoField(serialize=False, verbose_name='ID', auto_created=True, primary_key=True)),
                ('status', models.IntegerField()),
                ('cluster', models.CharField(max_length=250)),
                ('creation_date', models.DateTimeField(auto_now_add=True)),
                ('end_time', models.DateTimeField(null=True, blank=True)),
                ('user', models.ForeignKey(to=settings.AUTH_USER_MODEL)),
            ],
        ),
        migrations.AddField(
            model_name='password',
            name='task',
            field=models.ForeignKey(related_name='passwords', to='aplikacja_kliencka.Task'),
        ),
    ]
