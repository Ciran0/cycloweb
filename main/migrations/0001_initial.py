# Generated by Django 5.1.4 on 2025-01-09 02:58

from django.conf import settings
from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
    ]

    operations = [
        migrations.CreateModel(
            name='RobModel',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('name', models.CharField(max_length=100)),
                ('description', models.TextField(blank=True)),
                ('path', models.CharField(help_text='Path to the binary', max_length=255)),
                ('parameters', models.TextField(blank=True, help_text='JSON string for parameters')),
                ('allowed_users', models.ManyToManyField(blank=True, related_name='allowed_robmodels', to=settings.AUTH_USER_MODEL)),
            ],
        ),
    ]
