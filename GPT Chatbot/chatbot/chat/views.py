import openai
import logging
from django.conf import settings
from rest_framework.response import Response
from rest_framework.decorators import api_view
from chat.models import Conversation, Message
from chat.serializers import ConversationSerializer, MessageSerializer

logger = logging.getLogger(__name__)

@api_view(["POST"])
def chat_with_gpt(request):
    user_message = request.data.get("message")
    conversation_id = request.data.get("conversation_id")
    
    if not conversation_id:
        # print(f"Conversation ID: {conversation_id}")
        return Response({"error": "No conversation_id provided"}, status=400)
    conversation = Conversation.objects.get(id=conversation_id)
    Message.objects.create(conversation=conversation, role="user", text=user_message)
    try:
        #logger.info(f"User message: {user_message}")
        client = openai.OpenAI(api_key=settings.OPENAI_API_KEY)
        response = client.chat.completions.create(
            model="gpt-3.5-turbo",  # or GPT-4 
            messages=[
                #{"role": "system", "content": "You are a usefull assistant !"},
                {"role": "user", "content": user_message}
            ]
        )       
        bot_reply = response.choices[0].message.content
        #logger.info(f"Bot reply: {bot_reply}")       
        Message.objects.create(conversation=conversation, role="bot", text=bot_reply)        
        return Response({"reply": bot_reply})
    except Exception as e:
        #logger.error(f'Error OpenAI: {str(e)}')
        return Response({"error": str(e)}, status=500)
    
@api_view(["POST"])
def create_conversation(request):
    username = request.data.get("username")   
    conversation = Conversation.objects.create(username=username)
    set_conversations_names(username)
    conversation = Conversation.objects.get(id=conversation.id)
    
    return Response(ConversationSerializer(conversation).data)

@api_view(["GET"])
def get_conversations(request):
    username = request.GET.get("username")
    conversations = Conversation.objects.filter(username=username)
    serializer = ConversationSerializer(conversations, many=True)
    
    return Response(serializer.data)

@api_view(["GET"])
def get_messages(request):
    conversation_id = request.GET.get("conversation_id")
    conversation = Conversation.objects.get(id=conversation_id)
    messages = conversation.messages.all()
    
    return Response(MessageSerializer(messages, many=True).data)

@api_view(["DELETE"])
def delete_conversation(request, id):
    user = Conversation.objects.get(id=id).username
    Conversation.objects.get(id=id).delete()
    set_conversations_names(user)
    
    # return 204 No Content
    return Response(status=204)


def set_conversations_names(user):
    conversations = Conversation.objects.filter(username=user)
    for i, conversation in enumerate(conversations):
        conversation.name = f"Conversation {i + 1}"
        conversation.save()
        