from django.urls import path
from . import views

urlpatterns = [
    path('contact/', views.contact, name='contact'),
    path('portfolio/', views.portfolio, name='portfolio'),
    path('portfolio/<int:project_id>/', views.project, name='project'),
    path('about/', views.about, name='about'),
    path('', views.index, name='index'),
    path('resume/', views.resume, name='resume'),
    path('certificates/', views.certificates, name='certificates'),
]