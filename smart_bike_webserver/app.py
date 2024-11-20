from flask import Flask, render_template, request, jsonify

app = Flask(__name__)

# Simulated bike status and alerts
bike_status = {"state": "Locked", "alert": False}

@app.route('/')
def home():
    return render_template('index.html', bike_status=bike_status)

@app.route('/update_status', methods=['POST'])
def update_status():
    global bike_status
    data = request.json
    bike_status['state'] = data.get('state', bike_status['state'])
    bike_status['alert'] = data.get('alert', bike_status['alert'])
    return jsonify({"message": "Status updated successfully", "bike_status": bike_status})

@app.route('/get_status', methods=['GET'])
def get_status():
    return jsonify(bike_status)

if __name__ == '__main__':
    app.run(debug=True)
