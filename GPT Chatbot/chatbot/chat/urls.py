from django.urls import path
from .views import chat_with_gpt, create_conversation, get_conversations, delete_conversation, get_messages

urlpatterns = [
    path("chat/", chat_with_gpt, name="chat"),
    path("messages/", get_messages, name="messages"),
    path("conversations/", get_conversations, name="conversations"),
    path("conversation/", create_conversation, name="create_conversation"),
    path("conversation/<int:id>", delete_conversation, name="delete_conversation"),
]
