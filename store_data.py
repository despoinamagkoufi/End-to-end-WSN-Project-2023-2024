import pymongo
import re
from datetime import datetime

# MongoDB connection setup
try:
    client = pymongo.MongoClient("mongodb://localhost:27017/")
    db = client["sensor_database"]
    collection = db["sensor_data"]
    print("Connected to MongoDB successfully.")
except Exception as e:
    print(f"Error connecting to MongoDB: {e}")
    exit(1)

# Open the named pipe for reading
try:
    with open('out_pipe', 'r') as pipe:
        print("Reading from out_pipe and storing data in MongoDB...")

        while True:
            try:
                # Read line from the pipe
                line = pipe.readline().strip()
                
                # Skip if the line is empty
                if not line:
                    continue

                print(f"Line received: {line}")

                # Updated regular expression to handle timestamp and log format
                match = re.match(r"\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\] \[INFO: Parent\s*\] Received data from ID=([a-fA-F0-9]+): Count=(\d+), Temp=([0-9.]+) C, Hum=([0-9.]+) %RH", line)
                
                if match:
                    # Generate a timestamp for the current entry
                    timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

                    # Extract sensor data from the matched pattern
                    sensor_id = match.group(1)
                    count = int(match.group(2))
                    temperature = float(match.group(3))
                    humidity = float(match.group(4))

                    # Prepare the document to insert into MongoDB
                    document = {
                        "timestamp": timestamp,
                        "sensor_id": sensor_id,
                        "count": count,
                        "temperature": temperature,
                        "humidity": humidity
                    }

                    # Insert the document into MongoDB
                    collection.insert_one(document)
                    print(f"Inserted into MongoDB: {document}")
                else:
                    print(f"Unrecognized format: {line}")
            except Exception as read_error:
                print(f"Error reading or processing data: {read_error}")
except FileNotFoundError:
    print("Named pipe 'out_pipe' not found. Make sure the named pipe exists.")
except Exception as e:
    print(f"Error opening the named pipe: {e}")

