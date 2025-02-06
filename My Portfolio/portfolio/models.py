from django.db import models
import json

# Create your models here.


class Project(models.Model):
    title = models.CharField(max_length=100, verbose_name='Project Title', default='Project Title')
    description = models.TextField(default='Project Description')
    images = models.TextField(max_length=1000, default='Project Images')
    technologies = models.CharField(max_length=200, default='Technologies')
    category = models.CharField(max_length=50, default='Category')
    link = models.CharField(max_length=200, blank=True, default='#')

    def set_images(self, image_list):
        self.images = json.dumps(image_list)
        
    def get_images(self):
        return json.loads(self.images)

    def __str__(self):
        return self.title
