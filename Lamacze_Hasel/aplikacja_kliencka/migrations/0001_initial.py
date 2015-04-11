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
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('hash', models.CharField(max_length=250)),
                ('password', models.CharField(max_length=250)),
                ('start_time', models.DateTimeField(null=True)),
                ('end_time', models.DateTimeField(null=True)),
                ('algorithm', models.CharField(max_length=30)),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.CreateModel(
            name='Task',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('status', models.IntegerField()),
                ('cluster', models.CharField(max_length=250)),
                ('creation_date', models.DateTimeField(auto_now_add=True)),
                ('user', models.ForeignKey(to=settings.AUTH_USER_MODEL)),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.AddField(
            model_name='password',
            name='task',
            field=models.ForeignKey(related_name='passwords', to='aplikacja_kliencka.Task'),
            preserve_default=True,
        ),
    ]
