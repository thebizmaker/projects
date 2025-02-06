# Utiliser une image Python comme base
FROM python:3.11-slim

RUN apt-get update && apt-get install -y \
    build-essential \
    libpq-dev \
    && apt-get clean

# Définir le répertoire de travail dans le conteneur
WORKDIR /app

ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

RUN apt-get update && apt-get install -y \
    default-libmysqlclient-dev \
    gcc \
    && rm -rf /var/lib/apt/lists/*

# Copier les fichiers nécessaires
COPY . /app

# Installer les dépendances(y compris Guinicorn)
RUN pip install --upgrade pip
RUN pip install -r requirements.txt

# Exposer le port de l'application
EXPOSE 8080

# Commande de lancement de l'application
CMD ["sh", "-c", "python manage.py makemigrations && python manage.py migrate && gunicorn mywebsite.wsgi:application --bind 0.0.0.0:8080"]

