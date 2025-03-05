from flask import Flask, request, jsonify
from flask_cors import CORS
import requests
import mysql.connector
from datetime import datetime, timezone

app = Flask(__name__)
CORS(app) # This will enable CORS for all routes

# MySQL setup
db_config = {
    'user': 'Dester',
    'password': 'removed',
    'host': 'myhost',
    'database': 'chatbot',
    'port': '3306'
}

RASA_SERVER_URL = 'http://localhost:5005/webhooks/rest/webhook'

@app.route('/chat', methods=['POST'])
def chat():
    data = request.json
    print(f'Received data: {data}')
    user_message = data.get('message')
    user_id = data.get('user_id')
    conversation_id = data.get('conversation_id')
    
    if not user_message or not user_id or not conversation_id:
        return jsonify({'error': 'No message, user_id or conversation_id provided'}), 400
    
    print(f'Received message from user {user_id} in conversation {conversation_id}: {user_message}')
    
    # Send the user message to Rasa
    response = requests.post(
        RASA_SERVER_URL,
        json={
            'sender': user_id,
            'message': user_message
        }
    )
    
    if response.status_code != 200:
        print(f'Failed to send message to Rasa: {response.text}')
        return jsonify({'error': 'Failed to send message to Rasa'}), 500
    
    bot_response = response.json()
    print(f'Received response from Rasa: {bot_response}')
    
    # Save the conversation to the database
    save_chat_to_db(user_id, conversation_id, user_message, bot_response[0]['text'])
    
    return jsonify(bot_response)

@app.route('/history', methods=['GET'])
def chat_history():
    user_id = request.args.get('user_id')
    conversation_id = request.args.get('conversation_id')
    
    if not user_id or not conversation_id:
        return jsonify({'error': 'No user_id or conversation_id provided'}), 400
    
    history = get_chat_history(user_id, conversation_id)
    return jsonify(history)

@app.route('/new_conversation', methods=['POST'])
def new_conversation():
    data = request.json
    user_id = data.get('user_id')
    
    if not user_id:
        return jsonify({'error': 'No user_id provided'}), 400
    
    conversation_id = str(datetime.now(timezone.utc).timestamp())
    
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor()
    cursor.execute("""
        INSERT INTO conversations (user_id, conversation_id, timestamp)
        VALUES (%s, %s, %s)
        """, (user_id, conversation_id, datetime.now(timezone.utc)))
    conn.commit()
    cursor.close()
    conn.close()
    
    return jsonify({'conversation_id': conversation_id})

@app.route('/conversations', methods=['GET'])
def get_conversations():
    user_id = request.args.get('user_id')
    
    if not user_id:
        return jsonify({'error': 'No user_id provided'}), 400
    
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor(dictionary=True)
    cursor.execute("""
        SELECT conversation_id
        FROM conversations
        WHERE user_id = %s
        """, (user_id,))
    conversations = cursor.fetchall()
    cursor.close()
    conn.close
    
    return jsonify(conversations)

@app.route('/conversation', methods=['DELETE'])
def delete_conversation():
    data = request.json
    user_id = data.get('user_id')
    conversation_id = data.get('conversation_id')
    
    if not user_id or not conversation_id:
        return jsonify({'error': 'No user_id or conversation_id provided'}), 400
    
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor()
    cursor.execute("""
        DELETE FROM messages
        WHERE conversation_id = %s
        """, (conversation_id,))
    cursor.execute("""
        DELETE FROM conversations
        WHERE conversation_id = %s
        """, (conversation_id,))
    conn.commit()
    cursor.close()
    conn.close()
    
    return jsonify({'message': 'Conversation deleted'})

def get_chat_history(user_id, conversation_id):
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor(dictionary=True)
    cursor.execute("""
        SELECT user_message, bot_response
        FROM messages
        WHERE conversation_id = %s
        ORDER BY timestamp ASC
        """, (conversation_id,))
    history = cursor.fetchall()
    cursor.close()
    conn.close()
    print(f'Retrieved chat history for user {user_id} in conversation {conversation_id}: {history}')
    return history

def save_chat_to_db(user_id, conversation_id, user_message, bot_response):
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor()
    cursor.execute("""
        INSERT INTO messages (conversation_id, user_message, bot_response, timestamp)
        VALUES (%s, %s, %s, %s)
        """, (conversation_id, user_message, bot_response, datetime.now(timezone.utc)))
    conn.commit()
    cursor.close()
    conn.close()
    print(f'Saved message to database for user {user_id} in conversation {conversation_id}: {user_message} - {bot_response}')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5001, debug=True)