from flask import Flask, render_template, jsonify
from pymongo import MongoClient
import datetime

app = Flask(__name__)

# Connect to MongoDB (make sure the database and collection names match your setup)
client = MongoClient('mongodb://localhost:27017/')
db = client['sensor_database']  # This is your MongoDB database
collection = db['sensor_data']  # This is the collection where sensor data is stored

# Route to serve the main webpage
@app.route('/')
def index():
    return render_template('index.html')

# Route to serve real-time sensor data
@app.route('/data')
def get_data():
    # Fetch the most recent 10 records from MongoDB
    data = collection.find().sort([('timestamp', -1)]).limit(100)
    results = []
    for document in data:
        # Check if the timestamp is a string or a datetime object
        if isinstance(document['timestamp'], str):
            timestamp_str = document['timestamp']  # If it's a string, just use it
        else:
            timestamp_str = document['timestamp'].strftime('%Y-%m-%d %H:%M:%S')  # Convert if it's a datetime object
        
        results.append({
            'timestamp': timestamp_str,
            'temperature': document['temperature'],
            'humidity': document['humidity']
        })
    return jsonify(results)

# Route to retrieve historical data for a given time range
@app.route('/history/<start>/<end>/<sensor>')
def get_history(start, end,sensor):
    # Convert the received start and end to proper datetime format
    start_datetime = datetime.datetime.strptime(start, '%Y-%m-%dT%H:%M')
    end_datetime = datetime.datetime.strptime(end, '%Y-%m-%dT%H:%M')

    # Log the formatted timestamps for debugging
    print(f"Querying historical data from {start_datetime} to {end_datetime} from {sensor}")

    # Build the base query for the time range
    query = {
        'timestamp': {'$gte': start_datetime.strftime('%Y-%m-%d %H:%M:%S'),
                      '$lte': end_datetime.strftime('%Y-%m-%d %H:%M:%S')}
    }

    # Add sensor filtering based on the selected option
    if sensor == 'sensor1':
        query['sensor_id'] = '74a1'  # Filter for Sensor 1
    elif sensor == 'sensor2':
        query['sensor_id'] = '87f1'  # Filter for Sensor 2
    # If sensor is 'both', no sensor_id filter is applied

    # Query MongoDB using the time range and sensor filter
    data = collection.find(query).sort([('timestamp', -1)])

    results = []
    for document in data:
        #Append the fetched data into the results
        results.append({
            'timestamp': document['timestamp'],
            'temperature': document['temperature'],
            'humidity': document['humidity']
        })
    
    return jsonify(results)


# Run the app on localhost
if __name__ == '__main__':
    app.run(debug=True)
