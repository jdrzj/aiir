from django.conf.urls import patterns, url
from django.contrib import admin
from aplikacja_kliencka import views
from aplikacja_kliencka import mpi
#from aplikacja_kliencka import mpi_send
#from aplikacja_kliencka import mpi_listen


admin.autodiscover()

urlpatterns = patterns('',
    url(r'^$', views.home, name='home'),
    url(r'^login/$', views.login, name='login'),
    url(r'^logout/$', views.logout, name='logout'),
    url(r'^register/$', views.register, name='register'),
    url(r'^password_change/$', views.password_change, name='password_change'),
    url(r'^hash_generator/$', views.hash_generator, name='hash_generator'),
    url(r'^get_status/$', views.get_status, name='get_status'),
    url(r'^get_password_status/$', views.get_password_status, name='get_password_status'),
    url(r'^generate_hash/$', views.generate_hash, name='generate_hash'),
    url(r'^about_project/$', views.about_project, name='about_project'),
    url(r'^edit_profile/$', views.edit_profile, name='edit_profile'),
    #url(r'^mpi_send/$', mpi.send, name='mpi_send'),
    url(r'^add_task/$', views.add_task, name='add_task'),
    url(r'^task_actual/$', views.task_actual, name='task_actual'),
    url(r'^get_passwords/(?P<id>\w+)/$', views.get_passwords, name='get_passwords'),
    url(r'^task_history/$', views.task_history, name='task_history'),
    url(r'^task_details/(?P<id>\w+)/$', views.task_details, name='task_details'),
    url(r'^password_details/(?P<id>\w+)/$', views.password_details, name='password_details'),
    url(r'^add_tasks_from_file/', views.add_tasks_from_file, name='add_tasks_from_file'),
)
