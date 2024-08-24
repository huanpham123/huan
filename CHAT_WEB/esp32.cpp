// Load Wi-Fi library
#include "WiFi.h"
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid     = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Địa chỉ của server bạn đã xây dựng
char chatbot_server[] = "http://your_server_ip_or_domain";

WiFiServer server(80);
WiFiClient client1;

HTTPClient http;

String chatgpt_Q;
String chatgpt_A;
String json_String;
uint16_t dataStart = 0;
uint16_t dataEnd = 0;
String dataStr;
int httpCode = 0;

typedef enum 
{
  do_webserver_index,
  send_chatgpt_request,
  get_chatgpt_list,
} STATE_;

STATE_ currentState;

void WiFiConnect(void){
    WiFi.begin(ssid, password);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    currentState = do_webserver_index;
}

const char html_page[] PROGMEM = {
    // Nội dung HTML giữ nguyên như trước
};

void setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    while(!Serial);

    Serial.println("WiFi Setup done!");
    
    WiFiConnect();
    server.begin();
}

void loop()
{
  switch(currentState){
    case do_webserver_index:
      // Code giữ nguyên như trước
      break;
    case send_chatgpt_request:
      Serial.println("Ask Chatbot a Question Task Launch");
      if (http.begin(chatbot_server)) {  // HTTP
        http.addHeader("Content-Type", "application/json"); 
        String payload = String("{\"input\": \"") + chatgpt_Q + String("\"}");
        httpCode = http.POST(payload);   // start connection and send HTTP header
        payload = "";
        currentState = get_chatgpt_list;
      }
      else {
        Serial.println("[HTTP] Unable to connect");
        delay(1000);
      }
      break;
    case get_chatgpt_list:
      Serial.println("Get Chatbot Answers Task Launch");
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        chatgpt_A = payload;  // Giả định server trả về câu trả lời trực tiếp
        Serial.print("Chatbot Answer is: ");
        Serial.println(chatgpt_A);
        Serial.println("Wait 10s before next round...");
        currentState = do_webserver_index;
      }
      else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        while(1);
      }
      http.end();
      delay(10000);
      break;
  }
}




































// // Load Wi-Fi library
// #include "WiFi.h"
// #include <HTTPClient.h>

// // Replace with your network credentials
// const char* ssid     = "YOUR_SSID";
// const char* password = "YOUR_PASSWORD";

// // Địa chỉ của server chatbot
// char chatbot_server[] = "http://127.0.0.1:5000";

// // Thiết lập các đối tượng Wi-Fi và HTTP
// WiFiServer server(80);
// WiFiClient client1;
// HTTPClient http;

// String chatgpt_Q;  // Câu hỏi của người dùng
// String chatgpt_A;  // Câu trả lời từ chatbot
// String json_String;  // Chuỗi JSON để phân tích yêu cầu HTTP
// uint16_t dataStart = 0;
// uint16_t dataEnd = 0;
// String dataStr;
// int httpCode = 0;

// // Các trạng thái của chương trình
// typedef enum {
//   do_webserver_index,
//   send_chatgpt_request,
//   get_chatgpt_list,
// } STATE_;

// STATE_ currentState;

// // Hàm kết nối Wi-Fi
// void WiFiConnect(void) {
//     WiFi.begin(ssid, password);
//     Serial.print("Connecting to ");
//     Serial.println(ssid);
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//         Serial.print(".");
//     }
//     Serial.println("");
//     Serial.println("WiFi connected!");
//     Serial.print("IP address: ");
//     Serial.println(WiFi.localIP());
//     currentState = do_webserver_index;
// }

// // Trang HTML đơn giản để hiển thị trên web
// const char html_page[] PROGMEM = {
//     "HTTP/1.1 200 OK\r\n"
//     "Content-Type: text/html\r\n"
//     "Connection: close\r\n"
//     "\r\n"
//     "<!DOCTYPE HTML>\r\n"
//     "<html>\r\n"
//     "<head>\r\n"
//       "<meta charset=\"UTF-8\">\r\n"
//       "<title>Cloud Printer: ChatGPT</title>\r\n"
//       "<link rel=\"icon\" href=\"https://files.seeedstudio.com/wiki/xiaoesp32c3-chatgpt/chatgpt-logo.png\" type=\"image/x-icon\">\r\n"
//     "</head>\r\n"
//     "<body>\r\n"
//     "<img alt=\"SEEED\" src=\"https://files.seeedstudio.com/wiki/xiaoesp32c3-chatgpt/logo.png\" height=\"100\" width=\"410\">\r\n"
//     "<p style=\"text-align:center;\">\r\n"
//     "<img alt=\"ChatGPT\" src=\"https://files.seeedstudio.com/wiki/xiaoesp32c3-chatgpt/chatgpt-logo.png\" height=\"200\" width=\"200\">\r\n"
//     "<h1 align=\"center\">Cloud Printer</h1>\r\n" 
//     "<h1 align=\"center\">OpenAI ChatGPT</h1>\r\n" 
//     "<div style=\"text-align:center;vertical-align:middle;\">"
//     "<form action=\"/\" method=\"post\">"
//     "<input type=\"text\" placeholder=\"Please enter your question\" size=\"35\" name=\"chatgpttext\" required=\"required\"/>\r\n"
//     "<input type=\"submit\" value=\"Submit\" style=\"height:30px; width:80px;\"/>"
//     "</form>"
//     "</div>"
//     "</p>\r\n"
//     "</body>\r\n"
//     "<html>\r\n"
// };

// void setup() {
//     Serial.begin(115200);
//     WiFi.mode(WIFI_STA);
//     WiFi.disconnect();
//     while(!Serial);

//     Serial.println("WiFi Setup done!");
    
//     WiFiConnect();
//     server.begin();  // Khởi động server TCP
// }

// void loop() {
//   switch(currentState) {
//     case do_webserver_index:
//       Serial.println("Web Production Task Launch");
//       client1 = server.available();  // Kiểm tra nếu có client kết nối
//       if (client1) {
//         Serial.println("New Client.");  // Thông báo client mới kết nối
//         boolean currentLineIsBlank = true;    
//         while (client1.connected()) {
//           if (client1.available()) {
//             char c = client1.read();  // Đọc dữ liệu từ client
//             json_String += c;
//             if (c == '\n' && currentLineIsBlank) {                                 
//               dataStr = json_String.substring(0, 4);
//               Serial.println(dataStr);
//               if (dataStr == "GET ") {
//                 client1.print(html_page);  // Gửi trang HTML tới client
//               } else if (dataStr == "POST") {
//                 json_String = "";
//                 while(client1.available()) {
//                   json_String += (char)client1.read();
//                 }
//                 Serial.println(json_String); 
//                 dataStart = json_String.indexOf("chatgpttext=") + strlen("chatgpttext="); // Lấy nội dung câu hỏi
//                 chatgpt_Q = json_String.substring(dataStart, json_String.length());                    
//                 client1.print(html_page);
//                 Serial.print("Your Question is: ");
//                 Serial.println(chatgpt_Q);
//                 client1.stop();  // Đóng kết nối client
//                 currentState = send_chatgpt_request;
//               }
//               json_String = "";
//               break;
//             }
//             if (c == '\n') {
//               currentLineIsBlank = true;
//             } else if (c != '\r') {
//               currentLineIsBlank = false;
//             }
//           }
//         }
//       }
//       delay(1000);
//       break;

//     case send_chatgpt_request:
//       Serial.println("Ask Chatbot a Question Task Launch");
//       if (http.begin(chatbot_server)) {  // Bắt đầu kết nối đến server
//         http.addHeader("Content-Type", "application/json"); 
//         String payload = String("{\"input\": \"") + chatgpt_Q + String("\"}");
//         httpCode = http.POST(payload);  // Gửi yêu cầu POST với payload
//         payload = "";
//         currentState = get_chatgpt_list;
//       } else {
//         Serial.println("[HTTP] Unable to connect");
//         delay(1000);
//       }
//       break;

//     case get_chatgpt_list:
//       Serial.println("Get Chatbot Answers Task Launch");
//       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
//         String payload = http.getString();  // Lấy phản hồi từ server
//         chatgpt_A = payload;  // Giả định server trả về câu trả lời trực tiếp
//         Serial.print("Chatbot Answer is: ");
//         Serial.println(chatgpt_A);
//         Serial.println("Wait 10s before next round...");
//         currentState = do_webserver_index;
//       } else {
//         Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//         while(1);
//       }
//       http.end();
//       delay(10000);
//       break;
//   }
// }