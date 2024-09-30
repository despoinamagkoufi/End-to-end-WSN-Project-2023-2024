let realTimeUpdateId; 

// Function to update the real-time graph
function updateGraph() {
    fetch('/data')
        .then(response => response.json())
        .then(data => {
            let timestamps = data.map(d => d.timestamp);
            let temperatures = data.map(d => d.temperature);
            let humidities = data.map(d => d.humidity);

            let temperatureTrace = {
                x: timestamps,
                y: temperatures,
                mode: 'lines',
                name: 'Temperature (C)'
            };

            let humidityTrace = {
                x: timestamps,
                y: humidities,
                mode: 'lines',
                name: 'Humidity (%)'
            };

            let layout = {
                title: 'Real-Time Sensor Data',
                xaxis: { title: 'Timestamp' },
                yaxis: { title: 'Value' }
            };

            Plotly.newPlot('graph', [temperatureTrace, humidityTrace], layout);
        });
}

// Update the graph every 5 seconds
realTimeUpdateId = setInterval(updateGraph, 5000);

// Handle form submission for historical data
document.getElementById('historyForm').addEventListener('submit', function(event) {
    event.preventDefault();


    let start = document.getElementById('start').value;
    let end = document.getElementById('end').value;
    let sensor = document.getElementById('sensor').value;

    clearInterval(realTimeUpdateId); // Stop the real-time updates when fetching historical data

    fetch(`/history/${start}/${end}/${sensor}`)
        .then(response => response.json())
        .then(data => {
            let timestamps = data.map(d => d.timestamp);
            let temperatures = data.map(d => d.temperature);
            let humidities = data.map(d => d.humidity);

            // Plot historical data on the graph
            let temperatureTrace = {
                x: timestamps,
                y: temperatures,
                mode: 'lines',
                name: 'Temperature (C)'
            };

            let humidityTrace = {
                x: timestamps,
                y: humidities,
                mode: 'lines',
                name: 'Humidity (%)'
            };

            let layout = {
                title: 'Historical Sensor Data',
                xaxis: { title: 'Timestamp' },
                yaxis: { title: 'Value' }
            };

            // Update the graph with historical data
            Plotly.newPlot('graph', [temperatureTrace, humidityTrace], layout);


            let historyDiv = document.getElementById('history');
            historyDiv.innerHTML = '';
 
 

            data.forEach(d => {
                historyDiv.innerHTML += `<p>${d.timestamp} - Temp: ${d.temperature} C, Hum: ${d.humidity} %</p>`;
            });
        });
});


