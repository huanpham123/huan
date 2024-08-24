from flask import Flask, request, jsonify, send_from_directory
from flask_cors import CORS
from g4f.client import Client
import os

app = Flask(__name__)
CORS(app)
client = Client()

@app.route('/')
def index():
    return send_from_directory('.', 'index.html')

@app.route('/chat', methods=['POST'])
def chat():
    data = request.json
    user_input = data.get('message')
    response = client.chat.completions.create(
        model="gpt-4",
        messages=[{"role": "user", "content": user_input}],
    )
    reply = response.choices[0].message.content
    return jsonify({'reply': reply})

if __name__ == '__main__':
    app.run(debug=True)