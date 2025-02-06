from django.shortcuts import render
from django.http import HttpResponse
from .forms import ContactForm
from django.core.mail import send_mail
from django.conf import settings
from .models import Project
import random




def contact(request):
    if request.method == 'POST':
        form = ContactForm(request.POST)
        if form.is_valid():
            name = form.cleaned_data['name']
            email = form.cleaned_data['email']
            subject = form.cleaned_data['subject']
            message = form.cleaned_data['message']
            full_message = f'You have a new message from {name} <{email}>\n\n{message}'
            send_mail(
                subject=subject,
                message=full_message,
                from_email=settings.CONTACT_EMAIL,
                recipient_list=[settings.CONTACT_EMAIL],
                fail_silently=False,
            )
            return HttpResponse('OK', status=200)
        return HttpResponse('Invalid form !', status=400)
    else:
        form = ContactForm()
    return render(request, 'contact.html', {'form': form})


def portfolio(request):
    projects = Project.objects.all()
    projects = random.sample(list(projects), len(projects))
    return render(request, 'portfolio.html', {'projects': projects})

def project(request, project_id):
    project = Project.objects.get(id=project_id)
    return render(request, 'portfolio-details.html', {'project': project})

def about(request):
    return render(request, 'about.html')

def index(request):
    return render(request, 'index.html')

def resume(request):
    return render(request, 'resume.html')

def certificates(request):
    return render(request, 'certificates.html')


            
