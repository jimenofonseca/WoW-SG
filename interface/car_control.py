import serial as sr
import numpy as np
import matplotlib.pyplot as plt
import bluetooth
import subprocess
import json
import dash
import dash_core_components as dcc
import dash_html_components as html
import dash_daq as daq
import time
from collections import deque
import plotly.graph_objs as go
import datetime
import pandas as pd
import random
import os


app = dash.Dash('vehicle-data')
max_length = 50
times = deque(maxlen=max_length)
temperatures = deque(maxlen=max_length)
humidities = deque(maxlen=max_length)
illuminances = deque(maxlen=max_length)

data_dict = {
    "Temperature [C]": temperatures,
    "Relative Humidity [%]": humidities,
    "Illuminance [Lux]": illuminances,
}

PORT = '/dev/cu.BT_SENSOR-SPPDev'
ADDRESS = "98-d3-31-f9-52-0a"
BITRATE = 9600
stream = os.popen('blueutil --pair n')
s = sr.Serial(PORT, BITRATE, timeout=0.1)


def read_values_from_arduino(times, temperatures, humidities, illuminances):
    # set time
    times.append(time.time())
    # extract values
    try:
        reading_str = s.readline().decode()
        reading_dict = json.loads(reading_str)
        temperatures.append(reading_dict["T_C"])
        humidities.append(reading_dict["RH_%"])
        illuminances.append(reading_dict["I_lux"])
    except:
        temperatures.append(np.nan)
        humidities.append(np.nan)
        illuminances.append(np.nan)

    print(temperatures[-1])
    return times, temperatures, humidities, illuminances

#
times, \
temperatures, \
humidities, \
illuminances, = read_values_from_arduino(times, temperatures, humidities, illuminances)

app.layout = html.Div([
    html.Div([html.H2('Vehicle Data', style={'float': 'left', }), ]),
    html.Div([
        daq.Joystick(
            id='my-joystick',
            label="Default",
            angle=0,
            force=0,
        ),
        html.Div(id='joystick-output')
    ]),
    dcc.Dropdown(id='data-name',
                 options=[{'label': s, 'value': s}
                          for s in data_dict.keys()],
                 value=['Temperature [C]', 'Relative Humidity [%]', 'Illuminance [Lux]'],
                 multi=True
                 ),
    html.Div(children=html.Div(id='graphs'), className='row'),
    dcc.Interval(
        id='graph-update',
        interval=2000,
        n_intervals=0)],
    className="container", style={'width': '98%', 'margin-left': 10, 'margin-right': 10, 'max-width': 50000})


@app.callback(
    dash.dependencies.Output('graphs', 'children'),
    [dash.dependencies.Input('data-name', 'value'),
     dash.dependencies.Input('graph-update', 'n_intervals')]
)
def update_graph(data_names, n):
    graphs = []
    read_values_from_arduino(times, temperatures, humidities, illuminances)

    if len(data_names) > 2:
        class_choice = 'col s12 m6 l4'
    elif len(data_names) == 2:
        class_choice = 'col s12 m6 l6'
    else:
        class_choice = 'col s12'

    data = []
    for data_name in data_names:
        data.append(go.Scatter(
            x=list(times),
            y=list(data_dict[data_name]),
            name=data_name,
            # fill="tozeroy",
            # fillcolor="#6897bb"
        ))

    graphs.append(
        html.Div(dcc.Graph(id='data',
                           animate=True,
                           figure={'data': data,
                                   'layout': go.Layout(xaxis=dict(range=[min(times), max(times)],
                                                                  tickmode='linear'),
                                                       yaxis=dict(autorange=True),
                                                       margin={'l': 50, 'r': 1, 't': 45, 'b': 1})}
                           ), className=class_choice))

    return graphs

@app.callback(
    dash.dependencies.Output('joystick-output', 'children'),
    [dash.dependencies.Input('my-joystick', 'angle'),
     dash.dependencies.Input('my-joystick', 'force')])
def update_output(angle, force):

    #send orders to controller
    #turn right
    if force > 0.01:
        # move rigth
        if (315 <= angle <= 360) or (0 <= angle <= 45):
            messagge =b"r"
        elif (0 <= angle <= 45):
            messagge =b"r"
        #move forward
        elif (45 < angle < 135):
            messagge =b"f"
        #move left
        elif (135 <= angle < 225):
            messagge =b"l"
        #move backwards
        elif (225 <= angle < 315):
            messagge = b"b"
    else:
        messagge = b"s"

    s.write(messagge)
    time.sleep(0.05)
    return ['Angle is {}'.format(angle),
            html.Br(),
            'Force is {}'.format(force)]

if __name__ == '__main__':
    app.run_server(debug=True, host='0.0.0.0')
