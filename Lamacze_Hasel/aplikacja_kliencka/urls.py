from django.conf.urls import patterns, url
from django.contrib import admin
from aplikacja_kliencka import views


admin.autodiscover()

urlpatterns = patterns('',
    url(r'^$', views.home, name='home'),
    url(r'^login/$', views.login, name='login'),
    url(r'^logout/$', views.logout, name='logout'),
    url(r'^register/$', views.register, name='register'),
    url(r'^password_change/$', views.password_change, name='password_change'),
    url(r'^hash_generator/$', views.hash_generator, name='hash_generator'),
    url(r'^generate_hash/$', views.generate_hash, name='generate_hash'),
    url(r'^about_project/$', views.about_project, name='about_project'),
    url(r'^edit_profile/$', views.edit_profile, name='edit_profile'),
)
