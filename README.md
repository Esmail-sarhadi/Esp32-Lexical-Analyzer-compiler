
**ESP32 File Upload and Lexical Analyzer**

This project utilizes an ESP32 microcontroller to create a simple web server that allows users to upload a file through a web interface. Once the file is uploaded, the server performs lexical analysis on its content using a DFA (Deterministic Finite Automaton) to identify different types of tokens such as keywords, identifiers, numbers, strings, and symbols. The results are then displayed in a tabular format on the web page.


## Features

- **File Upload**: Users can upload a file through a web interface.
- **Lexical Analysis**: Analyzes the uploaded file and identifies tokens using a DFA (Deterministic Finite Automaton).
- **Result Display**: Displays the analysis results in a formatted HTML table.

## Components Used

- **ESP32 Microcontroller**
- **Arduino IDE** or any compatible development environment for programming the ESP32

## Getting Started

### Prerequisites

- Arduino IDE installed on your computer
- ESP32 board package installed in the Arduino IDE
- Basic knowledge of HTML and C++ for understanding the code

### Installation

1. **Clone the Repository**

   ```bash
   git clone https://github.com/Esmail-sarhadi/Esp32-Lexical-Analyzer-compiler.git
   cd Esp32-Lexical-Analyzer-compiler
   ```

2. **Setup Arduino IDE**

   - Open the Arduino IDE.
   - Go to **File** -> **Open** and select the `main.ino` file from the cloned repository.

3. **Configure Wi-Fi Credentials**

   - Update the `ssid` and `password` variables in the code with your Wi-Fi network credentials.

4. **Upload the Code**

   - Connect your ESP32 board to your computer via USB.
   - Select the correct board and port from the **Tools** menu in the Arduino IDE.
   - Click on the **Upload** button.

5. **Access the Web Interface**

   - Open the Serial Monitor from the Arduino IDE.
   - Once the ESP32 connects to your Wi-Fi network, note the IP address displayed in the Serial Monitor.
   - Open a web browser and enter the IP address to access the file upload interface.

## Usage

1. Go to the web interface using the IP address provided by the ESP32.
2. Use the file upload form to select and upload a file.
3. The file content will be analyzed, and the results will be displayed in a table format on the web page.

## Code Explanation

- **index_html**: Contains the HTML for the file upload form.
- **analyzeText()**: Function to perform lexical analysis on the uploaded file content.
- **handleUpload()**: Function to handle file upload and trigger lexical analysis.
- **setup()**: Initializes Wi-Fi connection and sets up the web server.
- **loop()**: Continuously handles incoming client requests.

## Contributing

Feel free to fork the repository and submit pull requests. Contributions are welcome to improve the functionality and features of this project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [Arduino IDE](https://www.arduino.cc/en/software)

<h2 id="donation">Donation</h2>

<p>If you find this project helpful, consider making a donation:</p>
<p><a href="https://nowpayments.io/donation?api_key=REWCYVC-A1AMFK3-QNRS663-PKJSBD2&source=lk_donation&medium=referral" target="_blank">
     <img src="https://nowpayments.io/images/embeds/donation-button-black.svg" alt="Crypto donation button by NOWPayments">
</a></p>
