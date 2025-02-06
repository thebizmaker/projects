from django.contrib import admin
from django import forms
from .models import Project
import json

# Register your models here.


class ProjectAdminForm(forms.ModelForm):
    class Meta:
        model = Project
        # fields = '__all__'
        exclude = ('images',)
        
    images_list = forms.CharField(
        label='lien des images',
        widget=forms.Textarea,
        help_text='Enter one image URL per line',
    )
    
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        if self.instance and self.instance.pk:
            images = self.instance.get_images()
            self.fields['images_list'].initial = "\n".join(images)

    def clean_images_list(self):
        data = self.cleaned_data['images_list']
        return json.dumps(data.splitlines())
    
    
class ProjectAdmin(admin.ModelAdmin):
    form = ProjectAdminForm
    list_display = ('title', 'category', 'technologies')
    search_fields = ('title', 'category', 'technologies')
    
    
    def save_model(self, request, obj, form, change):
        obj.images = form.cleaned_data['images_list']
        super().save_model(request, obj, form, change)
        
        
admin.site.register(Project, ProjectAdmin)
