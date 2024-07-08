#include <WiFi.h>
#include <WebServer.h>
#include <vector>

// Your network credentials
const char* ssid = "charon";
const char* password = "12121212";

WebServer server(80);

struct Token {
  String type;
  String value;
};

// HTML content
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 File Upload</title>
</head>
<body>
    <h2>Upload File</h2>
    <form method="POST" action="/upload" enctype="multipart/form-data">
        <input type="file" name="file">
        <input type="submit" value="Upload">
    </form>
</body>
</html>
)rawliteral";

// DFA for lexical analysis
std::vector<Token> analyzeText(const String& text) {
  std::vector<Token> tokens;
  int length = text.length();
  int i = 0;

  while (i < length) {
    // Skip whitespace
    if (isspace(text[i])) {
      i++;
      continue;
    }

    // Single line comment
    if (text[i] == '/' && i + 1 < length && text[i + 1] == '/') {
      while (i < length && text[i] != '\n') {
        i++;
      }
      continue;
    }

    // Multi line comment
    if (text[i] == '/' && i + 1 < length && text[i + 1] == '*') {
      i += 2;
      while (i + 1 < length && !(text[i] == '*' && text[i + 1] == '/')) {
        i++;
      }
      if (i + 1 < length) {
        i += 2;
      }
      continue;
    }

    // Identifier or keyword
    if (isalpha(text[i]) || text[i] == '_') {
      int start = i;
      while (i < length && (isalnum(text[i]) || text[i] == '_')) {
        i++;
      }
      String word = text.substring(start, i);
      if (word == "return" || word == "double" || word == "int" || 
          word == "const" || word == "void" || word == "namespace" ||
          word == "class" || word == "char" || word == "string" ||
          word == "while" || word == "continue" || word == "break" ||
          word == "bool" || word == "for") {
        tokens.push_back({"Keyword", word});
      } else {
        tokens.push_back({"ID", word});
      }
      continue;
    }

    // Number
    if (isdigit(text[i]) || (text[i] == '.' && i+1 < length && isdigit(text[i+1]))) {
      int start = i;
      while (i < length && (isdigit(text[i]) || text[i] == '.')) {
        i++;
      }
      if (i < length && (text[i] == 'e' || text[i] == 'E') && 
          (i+1 < length && (text[i+1] == '+' || text[i+1] == '-' || isdigit(text[i+1])))) {
        i++;
        if (text[i] == '+' || text[i] == '-') i++;
        while (i < length && isdigit(text[i])) {
          i++;
        }
      }
      tokens.push_back({"Number", text.substring(start, i)});
      continue;
    }

    // String
    if (text[i] == '"') {
      int start = i;
      i++;
      while (i < length && text[i] != '"') {
        i++;
      }
      if (i < length && text[i] == '"') {
        i++;
      }
      tokens.push_back({"String", text.substring(start, i)});
      continue;
    }

    // Character
    if (text[i] == '\'') {
      int start = i;
      i++;
      while (i < length && text[i] != '\'') {
        i++;
      }
      if (i < length && text[i] == '\'') {
        i++;
      }
      tokens.push_back({"Char", text.substring(start, i)});
      continue;
    }

    // Symbols and operators
    if (ispunct(text[i])) {
      String sym(1, text[i]);

      if (i+1 < length) {
        String sym2 = text.substring(i, i+2);
        if (sym2 == "&&" || sym2 == "||" || sym2 == "==" || sym2 == "!=" ||
            sym2 == "<=" || sym2 == ">=" || sym2 == "+=" || sym2 == "-=" ||
            sym2 == "*=" || sym2 == "/=") {
          tokens.push_back({"Compound Operator", sym2});
          i += 2;
          continue;
        }
      }

      if (sym == "+" || sym == "-" || sym == "*" || sym == "/" || sym == "%") {
        tokens.push_back({"Arithmetic Operator", sym});
      } else if (sym == "^") {
        tokens.push_back({"Exponential Operator", sym});
      } else if (sym == "<" || sym == ">" || sym == "=" || sym == "!") {
        tokens.push_back({"Relational Operator", sym});
      } else if (sym == "&" || sym == "|" || sym == "!") {
        tokens.push_back({"Logical Operator", sym});
      } else if (sym == "," || sym == ";" || sym == ":" || sym == "(" ||
                 sym == ")" || sym == "{" || sym == "}" || sym == "[" || sym == "]") {
        tokens.push_back({"Symbol", sym});
      } else {
        tokens.push_back({"Unknown", sym});
      }
      i++;
      continue;
    }

    // Unknown token
    tokens.push_back({"Unknown", String(1, text[i])});
    i++;
  }

  return tokens;
}

// Function to handle file upload and lexical analysis
void handleUpload() {
  HTTPUpload& upload = server.upload();
  static String fileContent;

  if (upload.status == UPLOAD_FILE_START) {
    fileContent = "";
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    fileContent += String((char*)upload.buf).substring(0, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    std::vector<Token> tokens = analyzeText(fileContent);
    String result = "<html><body><h2>Analysis Result</h2><table border='1'><tr><th>Type</th><th>Value</th></tr>";
    for (const auto& token : tokens) {
        result += "<tr><td>" + token.type + "</td><td>" + token.value + "</td></tr>";
    }
    result += "</table></body></html>";
    server.send(200, "text/html", result);
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Serve index.html
  server.on("/", HTTP_GET, [](){
    server.send_P(200, "text/html", index_html);
  });

  // Handle file upload
  server.on("/upload", HTTP_POST, [](){
    server.send(200); 
  }, handleUpload);

  // Start server
  server.begin();
}

void loop() {
  server.handleClient();
}
