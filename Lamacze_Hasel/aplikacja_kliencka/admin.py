from django.contrib import admin
from aplikacja_kliencka.models import *

class TaskAdmin(admin.ModelAdmin):

    fieldsets = [
        (None, {'fields': ['user', 'status','cluster','end_time']}),
    ]
    list_display = ('user', 'status','cluster','creation_date','end_time')

class PasswordAdmin(admin.ModelAdmin):

    fieldsets = [
        (None, {'fields': ['hash', 'password', 'task', 'status', 'start_time', 'end_time', 'algorithm',
                           'password_cracking_algorithm']}),
        ]
    list_display = ('hash', 'password', 'task', 'status', 'start_time', 'end_time', 'algorithm', 'password_cracking_algorithm')

admin.site.register(Task, TaskAdmin)
admin.site.register(Password,PasswordAdmin)