// script.js
document.getElementById('send_button').addEventListener('click', async function() {
    const userInput = document.getElementById('user_input').value;
    const responseArea = document.getElementById('response_area');

    if (userInput.trim() === '') {
        responseArea.textContent = 'Vui lòng nhập câu hỏi.';
        return;
    }

    responseArea.textContent = 'Đang tải...';

    try {
        const response = await fetch('/chat', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ message: userInput })
        });

        const data = await response.json();
        responseArea.textContent = data.reply;
    } catch (error) {
        responseArea.textContent = 'Lỗi: ' + error.message;
    }
});
