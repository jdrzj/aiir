from django import forms
from django.contrib.auth.models import User
from aplikacja_kliencka.models import Password, Task
from django.utils.translation import gettext as _
from collections import OrderedDict
from django.core.exceptions import ValidationError

from django.core.exceptions import ValidationError

def validate_length(password):
    if len(password) < 8:
        raise ValidationError("Hasło powinno mieć co najmniej 8 znaków")

def validate_number(password):
    jestLiczba = any(char.isdigit() for char in password)
    if not jestLiczba:
        raise ValidationError("Hasło powinno mieć co najmniej 1 cyfrę")

def validate_space(password):
    jestSpacja = any(char == " " for char in password)
    if jestSpacja:
        raise ValidationError("Hasło nie może zawierać spacji")


class UserForm(forms.ModelForm):

    username = forms.CharField(help_text="Login:")
    password = forms.CharField(widget=forms.PasswordInput(), help_text="Hasło:", validators=[validate_length,
                                                                                             validate_number,
                                                                                             validate_space])
    email = forms.EmailField(help_text="E-Mail:")
    first_name = forms.CharField(help_text="Imię:")
    last_name = forms.CharField(help_text="Nazwisko:")
    class Meta:
        model = User
        fields = ('username','password','email', 'first_name', 'last_name')

class SetPasswordForm(forms.Form):

    error_messages = {
        'password_mismatch': _("Dwa pola nowego hasła nie pasują do siebie."),
    }
    new_password1 = forms.CharField(label=_("Nowe hasło"),
                                    widget=forms.PasswordInput, validators=[validate_length,
                                                                            validate_number,
                                                                            validate_space])
    new_password2 = forms.CharField(label=_("Potwierdź Nowe Hasło"),
                                    widget=forms.PasswordInput)

    def __init__(self, user, *args, **kwargs):
        self.user = user
        super(SetPasswordForm, self).__init__(*args, **kwargs)

    def clean_new_password2(self):
        password1 = self.cleaned_data.get('new_password1')
        password2 = self.cleaned_data.get('new_password2')
        if password1 and password2:
            if password1 != password2:
                raise forms.ValidationError(
                    self.error_messages['password_mismatch'],
                    code='password_mismatch',
                )
        return password2

    def save(self, commit=True):
        self.user.set_password(self.cleaned_data['new_password1'])
        if commit:
            self.user.save()
        return self.user

class PasswordChangeForm(SetPasswordForm):

    error_messages = dict(SetPasswordForm.error_messages, **{
        'password_incorrect': _("Podano nieprawidłowe hasło ! "
                                "Ponów próbę."),
    })
    old_password = forms.CharField(label=_("Stare hasło"),
                                   widget=forms.PasswordInput)

    def clean_old_password(self):

        old_password = self.cleaned_data["old_password"]
        if not self.user.check_password(old_password):
            raise forms.ValidationError(
                self.error_messages['password_incorrect'],
                code='password_incorrect',
            )
        return old_password

PasswordChangeForm.base_fields = OrderedDict(
    (k, PasswordChangeForm.base_fields[k])
    for k in ['old_password', 'new_password1', 'new_password2']
)

class EditProfileForm(forms.ModelForm):
    email = forms.EmailField(help_text="E-Mail:")
    first_name = forms.CharField(help_text="Imię:")
    last_name = forms.CharField(help_text="Nazwisko:")
    class Meta:
        model = User
        fields = ('email', 'first_name', 'last_name')

class PasswordAddForm(forms.ModelForm):

    password = forms.CharField(help_text="Hasło:", required=True)
    hash = forms.CharField(help_text="Hash:", required=True)
    algorithm = forms.CharField(help_text="Algorytm", required=True)

    class Meta:
        model = Password
        fields = ('password', 'hash', 'algorithm')

class TaskAddForm(forms.ModelForm):
    cluster = forms.IntegerField(help_text="Klaster", required=True)

    class Meta:
        model = Task
        fields = ('cluster',)