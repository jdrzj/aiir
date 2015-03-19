from django.contrib.auth import authenticate, update_session_auth_hash, login as auth_login, logout as auth_logout
from django.core.urlresolvers import reverse
from django.contrib.auth.decorators import login_required
from django.views.decorators.debug import sensitive_post_parameters
from django.views.decorators.csrf import csrf_protect, csrf_exempt
from django.shortcuts import render, render_to_response, RequestContext, HttpResponse, HttpResponseRedirect

import collections
import json
import hashlib

from aplikacja_kliencka.forms import *

def home(request):
    return render(request, "aplikacja_kliencka/index.html")

def hash_generator(request):
    return render(request, "aplikacja_kliencka/hash_generator.html")

def register(request):
    error_message = collections.OrderedDict()
    context = {}

    if request.method == 'POST':
        user_form = UserForm(data=request.POST)

        if user_form.is_valid() and request.POST['password'] == request.POST['password_repeat']:
            user_form.save()
            return render(request, "aplikacja_kliencka/register_success.html", context)

        else:
            if 'username' in user_form.errors.keys():
                error_message['Login'] = str(user_form.errors['username'])
            if 'password' in user_form.errors.keys():
                error_message['Hasło'] = str(user_form.errors['password'])
            if 'first_name' in user_form.errors.keys():
                error_message['Imię'] = str(user_form.errors['first_name'])
            if 'last_name' in user_form.errors.keys():
                error_message['Nazwisko'] = str(user_form.errors['last_name'])
            if 'email' in user_form.errors.keys():
                error_message['E-Mail'] = "Niepoprawny adres e-mail"
            for key in error_message:
                error_message[key] = (str(error_message[key])).replace('<ul class="errorlist"><li>', '')
                error_message[key] = (str(error_message[key])).replace('</li></ul>', '')
                error_message[key] = (str(error_message[key])).replace('</li><li>', ', ')

            if request.POST['password'] != request.POST['password_repeat']:
                error_message['Potwierdzenie Hasła'] = "Hasła nie pasują do siebie, spróbuj ponownie"
            context['error_message'] = error_message

    return render(request, "aplikacja_kliencka/register.html", context )

@csrf_protect
def login(request):
    context = {}
    if request.method == 'POST':
        username = request.POST['username']
        password = request.POST['password']

        requested_username = authenticate(username=username, password=password)

        if requested_username:
            if requested_username.is_active:
                auth_login(request, requested_username)
                return HttpResponseRedirect(reverse('aplikacja_kliencka:home'))
            else:
                context['error_message'] = "Twoje konto jest zablokowane"
        else:
            context['error_message'] = "Podano błędne dane do logowania, spróbuj ponownie !"

    return render(request, "aplikacja_kliencka/login.html", context)

@login_required
def logout(request):
    auth_logout(request)
    return HttpResponseRedirect(reverse('aplikacja_kliencka:home'))

@sensitive_post_parameters()
@csrf_protect
@login_required
def password_change(request):

    context = {}
    context["form"] = PasswordChangeForm(request.user)
    error_message = collections.OrderedDict()
    password_changed = False

    if request.method == 'POST':
        new_password_form = PasswordChangeForm(user=request.user, data=request.POST)
        if new_password_form.is_valid():
            new_password_form.save()
            update_session_auth_hash(request, new_password_form.user)
            password_changed = True
        else:
            if 'old_password' in new_password_form.errors.keys():
                error_message['Stare Hasło'] = str(new_password_form.errors['old_password'])
            if 'new_password1' in new_password_form.errors.keys():
                error_message['Nowe Hasło'] = str(new_password_form.errors['new_password1'])
            if 'new_password2' in new_password_form.errors.keys():
                error_message['Potwierdź Nowe Hasło'] = str(new_password_form.errors['new_password2'])

            for key in error_message:
                error_message[key] = (str(error_message[key])).replace('<ul class="errorlist"><li>', '')
                error_message[key] = (str(error_message[key])).replace('</li></ul>', '')
                error_message[key] = (str(error_message[key])).replace('</li><li>', ', ')

    context['password_changed'] = password_changed
    context['error_message'] = error_message

    return render(request, 'aplikacja_kliencka/password_change.html', context)


@csrf_exempt
def generate_hash(request):

    response_data = {}

    if request.is_ajax():
        try:
            requested_text_to_hash = request.POST['text_to_hash']
        except:
            response_data['status'] = "error"
            response_data['result'] = "We're sorry, but something went wrong."
        else:
            response_data['status'] = "success"
            response_data['result'] = "Your has is generated"
            response_data['text_length'] = len(requested_text_to_hash)
            response_data['sha1'] = str(hashlib.sha1((requested_text_to_hash).encode("utf8")).hexdigest())
            response_data['md5'] = str(hashlib.md5(requested_text_to_hash.encode("utf8")).hexdigest())
    else:
        response_data['status'] = "error"
        response_data['result'] = "We're sorry, but something went wrong"

    return HttpResponse(json.dumps(response_data), content_type='application/json')

def about_project(request):
    return render(request, "aplikacja_kliencka/about_project.html")

@login_required
def edit_profile(request):

    error_message = collections.OrderedDict()
    current_user = request.user
    message = ""
    if request.method == 'POST':
        edit_profile_form = EditProfileForm(request.POST, instance=current_user)

        if edit_profile_form.is_valid():
            edit_profile_form.save()

        else:
            if 'first_name' in edit_profile_form.errors.keys():
                error_message['first_name'] = str(edit_profile_form.errors['first_name'])
            if 'last_name' in edit_profile_form.errors.keys():
                error_message['last_name'] = str(edit_profile_form.errors['last_name'])
            if 'email' in edit_profile_form.errors.keys():
                error_message['E-Mail'] = "Niepoprawny adres e-mail"
            for key in error_message:
                error_message[key] = (str(error_message[key])).replace('<ul class="errorlist"><li>', '')
                error_message[key] = (str(error_message[key])).replace('</li></ul>', '')

    edit_profile_form = EditProfileForm(instance=current_user)

    context = {'edit_profile_form': edit_profile_form,
               'error_message': error_message}
    context['fistname'] = request.user.first_name
    context['lastname'] = request.user.last_name
    context['email'] = request.user.email
    return render(request, 'aplikacja_kliencka/edit_profile.html', context)