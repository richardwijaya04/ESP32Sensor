 from flask import Flask, request, jsonify
import time
import pandas as pd

app = Flask('Flask_Testing')

temperature = []
humidity = []
timestamp = []

@app.route('/', methods=['GET'])
def hello_world():
    if len(temperature) > 0:
        data_dict = {'timestamp': timestamp, 'temperature': temperature, 'humidity': humidity}
        df = pd.DataFrame(data_dict)
        return df.to_html()
    else:
        return 'No Data Received'

@app.route('/data', methods=['POST'])
def data():
    if request.is_json:
        data = request.json
        temp = data.get('temperature')
        hum = data.get('humidity')
    else:
        temp = request.form.get('temperature')
        hum = request.form.get('humidity')

    timing = time.time()
    temperature.append(temp)
    humidity.append(hum)
    timestamp.append(timing)

    return jsonify({'temperature': temp, 'humidity': hum, 'timestamp': timing})

if __name__ == '__main__':
    app.run(host='0.0.0.0')
