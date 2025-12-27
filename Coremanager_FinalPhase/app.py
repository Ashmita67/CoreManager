import dash
from dash import dcc, html, Input, Output, dash_table
import plotly.graph_objs as go
import plotly.express as px
import pandas as pd
import json
import os
from datetime import datetime
import numpy as np
from sklearn.linear_model import LinearRegression

# Initialize Dash app
app = dash.Dash(__name__, suppress_callback_exceptions=True)
app.title = "CoreManager 2.0 Dashboard"

# Color scheme
COLORS = {
    'background': '#1e1e1e',
    'card': '#2d2d2d',
    'text': '#ffffff',
    'primary': '#4a9eff',
    'success': '#4caf50',
    'warning': '#ff9800',
    'danger': '#f44336',
    'cores': ['#4a9eff', '#9c27b0', '#00bcd4', '#4caf50', '#ff9800', '#f44336', '#e91e63', '#009688']
}

# Default stats structure if file doesn't exist
DEFAULT_STATS = {
    "timestamp": datetime.now().isoformat(),
    "energy_mode": "Balanced",
    "cores": [
        {"id": 0, "utilization": 0, "tasks": []},
        {"id": 1, "utilization": 0, "tasks": []},
        {"id": 2, "utilization": 0, "tasks": []},
        {"id": 3, "utilization": 0, "tasks": []}
    ],
    "tasks": [],
    "migrations": []
}

def load_stats():
    """Load stats from JSON file, return default if not found"""
    try:
        if os.path.exists('data/stats.json'):
            with open('data/stats.json', 'r') as f:
                return json.load(f)
    except Exception as e:
        print(f"Error loading stats.json: {e}")
    return DEFAULT_STATS

def load_usage_log():
    """Load CPU usage log from CSV"""
    try:
        if os.path.exists('data/usage_log.csv'):
            df = pd.read_csv('data/usage_log.csv')
            return df
    except Exception as e:
        print(f"Error loading usage_log.csv: {e}")
    return pd.DataFrame()

def predict_load(df, model_path='data/model.txt'):
    """Predict future load using AI model"""
    if df.empty or len(df) < 3:
        return None
    
    try:
        # Load model coefficients
        if os.path.exists(model_path):
            with open(model_path, 'r') as f:
                coeffs = [float(line.strip()) for line in f.readlines()]
            
            if len(coeffs) >= 4 and 'core0' in df.columns and 'core1' in df.columns and 'core2' in df.columns:
                # Use last 3 cores to predict core3
                last_row = df.iloc[-1]
                X = [[last_row.get('core0', 0), last_row.get('core1', 0), last_row.get('core2', 0)]]
                prediction = coeffs[0] * X[0][0] + coeffs[1] * X[0][1] + coeffs[2] * X[0][2] + coeffs[3]
                return max(0, min(100, prediction))  # Clamp between 0-100
    except Exception as e:
        print(f"Error in prediction: {e}")
    return None

# App layout
app.layout = html.Div([
    dcc.Interval(
        id='interval-component',
        interval=3*1000,  # Update every 3 seconds
        n_intervals=0
    ),
    
    # Header
    html.Div([
        html.H1("🚀 CoreManager 2.0 Dashboard", style={
            'color': COLORS['text'],
            'margin': '0',
            'fontSize': '2.5em'
        }),
        html.P("AI-Driven CPU Scheduling Simulator", style={
            'color': '#aaa',
            'margin': '0.5em 0',
            'fontSize': '1.2em'
        })
    ], style={
        'background': COLORS['card'],
        'padding': '20px',
        'borderRadius': '10px',
        'marginBottom': '20px'
    }),
    
    # Energy Mode and System Status
    html.Div([
        html.Div([
            html.H3("⚡ Energy Mode", style={'margin': '0', 'color': COLORS['text']}),
            html.Div(id='energy-mode', style={
                'fontSize': '1.5em',
                'fontWeight': 'bold',
                'marginTop': '10px'
            })
        ], style={
            'background': COLORS['card'],
            'padding': '20px',
            'borderRadius': '10px',
            'flex': '1',
            'marginRight': '10px'
        }),
        html.Div([
            html.H3("📊 System Status", style={'margin': '0', 'color': COLORS['text']}),
            html.Div(id='system-status', style={
                'fontSize': '1em',
                'marginTop': '10px',
                'color': '#aaa'
            })
        ], style={
            'background': COLORS['card'],
            'padding': '20px',
            'borderRadius': '10px',
            'flex': '1',
            'marginLeft': '10px'
        })
    ], style={'display': 'flex', 'marginBottom': '20px'}),
    
    # CPU Core Utilization Chart
    html.Div([
        html.H3("📈 CPU Core Utilization", style={
            'color': COLORS['text'],
            'marginBottom': '20px'
        }),
        dcc.Graph(id='cpu-utilization-chart')
    ], style={
        'background': COLORS['card'],
        'padding': '20px',
        'borderRadius': '10px',
        'marginBottom': '20px'
    }),
    
    # Core Panels Grid
    html.Div([
        html.H3("🔧 CPU Cores", style={
            'color': COLORS['text'],
            'marginBottom': '20px',
            'width': '100%'
        }),
        html.Div(id='core-panels', style={
            'display': 'grid',
            'gridTemplateColumns': 'repeat(auto-fit, minmax(300px, 1fr))',
            'gap': '15px'
        })
    ], style={
        'background': COLORS['card'],
        'padding': '20px',
        'borderRadius': '10px',
        'marginBottom': '20px'
    }),
    
    # Tasks Table and Migrations
    html.Div([
        html.Div([
            html.H3("📋 Tasks", style={'color': COLORS['text'], 'marginBottom': '20px'}),
            html.Div(id='tasks-table')
        ], style={
            'flex': '1',
            'marginRight': '10px'
        }),
        html.Div([
            html.H3("🔄 Migration Events", style={'color': COLORS['text'], 'marginBottom': '20px'}),
            html.Div(id='migrations-log', style={
                'maxHeight': '400px',
                'overflowY': 'auto',
                'background': '#1a1a1a',
                'padding': '15px',
                'borderRadius': '5px',
                'fontFamily': 'monospace',
                'fontSize': '0.9em'
            })
        ], style={
            'flex': '1',
            'marginLeft': '10px'
        })
    ], style={
        'display': 'flex',
        'background': COLORS['card'],
        'padding': '20px',
        'borderRadius': '10px',
        'marginBottom': '20px'
    }),
    
    # AI Prediction Chart
    html.Div([
        html.H3("🤖 AI-Predicted Load Trend", style={
            'color': COLORS['text'],
            'marginBottom': '20px'
        }),
        dcc.Graph(id='ai-prediction-chart')
    ], style={
        'background': COLORS['card'],
        'padding': '20px',
        'borderRadius': '10px',
        'marginBottom': '20px'
    })
    
], style={
    'background': COLORS['background'],
    'padding': '20px',
    'minHeight': '100vh',
    'fontFamily': 'Arial, sans-serif'
})

# Callbacks
@app.callback(
    [Output('energy-mode', 'children'),
     Output('system-status', 'children'),
     Output('cpu-utilization-chart', 'figure'),
     Output('core-panels', 'children'),
     Output('tasks-table', 'children'),
     Output('migrations-log', 'children'),
     Output('ai-prediction-chart', 'figure')],
    [Input('interval-component', 'n_intervals')]
)
def update_dashboard(n):
    stats = load_stats()
    usage_df = load_usage_log()
    
    # Energy Mode
    energy_mode = stats.get('energy_mode', 'Balanced')
    energy_colors = {
        'Low': COLORS['success'],
        'Balanced': COLORS['primary'],
        'High': COLORS['warning']
    }
    energy_display = html.Span(energy_mode, style={'color': energy_colors.get(energy_mode, COLORS['text'])})
    
    # System Status
    cores = stats.get('cores', [])
    total_tasks = len(stats.get('tasks', []))
    active_tasks = sum(1 for t in stats.get('tasks', []) if not t.get('completed', False))
    avg_util = np.mean([c.get('utilization', 0) for c in cores]) if cores else 0
    status_text = f"Total Tasks: {total_tasks} | Active: {active_tasks} | Avg Utilization: {avg_util:.1f}%"
    
    # CPU Utilization Chart
    if not usage_df.empty and any(col.startswith('core') for col in usage_df.columns):
        core_cols = [col for col in usage_df.columns if col.startswith('core')]
        fig_util = go.Figure()
        for i, col in enumerate(core_cols):
            fig_util.add_trace(go.Bar(
                x=[col.upper()],
                y=[usage_df[col].iloc[-1] if len(usage_df) > 0 else 0],
                name=col.upper(),
                marker_color=COLORS['cores'][i % len(COLORS['cores'])]
            ))
        fig_util.update_layout(
            title="Current Core Utilization (%)",
            xaxis_title="CPU Core",
            yaxis_title="Utilization (%)",
            yaxis_range=[0, 100],
            template='plotly_dark',
            height=300,
            showlegend=False
        )
    else:
        # Use stats data if CSV is not available
        core_utils = [c.get('utilization', 0) for c in cores]
        fig_util = go.Figure()
        for i, util in enumerate(core_utils):
            fig_util.add_trace(go.Bar(
                x=[f'Core {i}'],
                y=[util],
                name=f'Core {i}',
                marker_color=COLORS['cores'][i % len(COLORS['cores'])]
            ))
        fig_util.update_layout(
            title="Current Core Utilization (%)",
            xaxis_title="CPU Core",
            yaxis_title="Utilization (%)",
            yaxis_range=[0, 100],
            template='plotly_dark',
            height=300,
            showlegend=False
        )
    
    # Core Panels
    core_panels = []
    for core in cores:
        core_id = core.get('id', 0)
        util = core.get('utilization', 0)
        tasks = core.get('tasks', [])
        
        # Color based on utilization
        if util < 30:
            util_color = COLORS['success']
        elif util < 70:
            util_color = COLORS['primary']
        else:
            util_color = COLORS['danger']
        
        task_list = html.Div([
            html.Div([
                html.Span(f"Task {t.get('id', '?')}", style={'fontWeight': 'bold'}),
                html.Span(f" | Prio: {t.get('priority', '?')}", style={'color': '#aaa', 'marginLeft': '10px'})
            ], style={'marginBottom': '5px'})
            for t in tasks
        ]) if tasks else html.P("No tasks assigned", style={'color': '#666', 'fontStyle': 'italic'})
        
        panel = html.Div([
            html.H4(f"Core {core_id}", style={'color': COLORS['text'], 'margin': '0 0 10px 0'}),
            html.Div([
                html.Span("Utilization: ", style={'color': '#aaa'}),
                html.Span(f"{util:.1f}%", style={'color': util_color, 'fontWeight': 'bold', 'fontSize': '1.2em'})
            ], style={'marginBottom': '15px'}),
            html.H5("Assigned Tasks:", style={'color': COLORS['text'], 'margin': '10px 0 5px 0', 'fontSize': '0.9em'}),
            task_list
        ], style={
            'background': '#1a1a1a',
            'padding': '15px',
            'borderRadius': '8px',
            'border': f'2px solid {COLORS["cores"][core_id % len(COLORS["cores"])]}'
        })
        core_panels.append(panel)
    
    # Tasks Table
    tasks = stats.get('tasks', [])
    if tasks:
        tasks_data = []
        for task in tasks:
            is_completed = task.get('completed', False)
            status_text = '✅ Completed' if is_completed else '🟢 Active'
            tasks_data.append({
                'ID': task.get('id', '?'),
                'Priority': task.get('priority', '?'),
                'Deadline': f"{task.get('deadline', 0):.2f}s",
                'Core': task.get('core_assigned', '?'),
                'Status': status_text,
                'CPU Time': f"{task.get('cpu_time', 0):.2f}s",
                '_completed': is_completed  # Hidden column for conditional styling
            })
        df_tasks = pd.DataFrame(tasks_data)
        
        # Only show visible columns (exclude _completed from display)
        visible_columns = [col for col in df_tasks.columns if not col.startswith('_')]
        
        # Create conditional styling using filter_query on the _completed column
        # This avoids emoji parsing issues in filter queries
        style_data_conditional = [
            {
                'if': {'filter_query': '{_completed} = true'},
                'backgroundColor': '#1a3a1a',
            },
            {
                'if': {'filter_query': '{_completed} = false'},
                'backgroundColor': '#3a3a1a',
            }
        ]
        
        table = dash_table.DataTable(
            data=df_tasks.to_dict('records'),  # Include _completed in data for filtering
            columns=[{'name': col, 'id': col} for col in visible_columns],  # But don't show it
            style_cell={
                'backgroundColor': '#1a1a1a',
                'color': COLORS['text'],
                'textAlign': 'left',
                'padding': '10px',
                'fontFamily': 'Arial'
            },
            style_header={
                'backgroundColor': COLORS['card'],
                'fontWeight': 'bold',
                'color': COLORS['text']
            },
            style_data_conditional=style_data_conditional
        )
    else:
        table = html.P("No tasks available", style={'color': '#666', 'fontStyle': 'italic'})
    
    # Migrations Log
    migrations = stats.get('migrations', [])
    if migrations:
        migration_items = []
        for mig in migrations[-10:]:  # Show last 10 migrations
            timestamp = mig.get('timestamp', '')
            task_id = mig.get('task_id', '?')
            from_core = mig.get('from_core', '?')
            to_core = mig.get('to_core', '?')
            migration_items.append(
                html.Div([
                    html.Span(f"[{timestamp}] ", style={'color': '#666'}),
                    html.Span(f"Task {task_id}: ", style={'color': COLORS['primary']}),
                    html.Span(f"Core {from_core} → Core {to_core}", style={'color': COLORS['warning']})
                ], style={'marginBottom': '8px', 'padding': '5px'})
            )
        migrations_display = html.Div(migration_items)
    else:
        migrations_display = html.P("No migrations recorded", style={'color': '#666', 'fontStyle': 'italic'})
    
    # AI Prediction Chart
    if not usage_df.empty:
        core_cols = [col for col in usage_df.columns if col.startswith('core')]
        if len(core_cols) >= 3:
            # Create time series
            usage_df_with_time = usage_df.copy()
            usage_df_with_time['time'] = range(len(usage_df_with_time))
            
            fig_pred = go.Figure()
            
            # Plot historical data
            for col in core_cols:
                fig_pred.add_trace(go.Scatter(
                    x=usage_df_with_time['time'],
                    y=usage_df_with_time[col],
                    mode='lines+markers',
                    name=col.upper(),
                    line=dict(color=COLORS['cores'][core_cols.index(col) % len(COLORS['cores'])])
                ))
            
            # Add prediction
            prediction = predict_load(usage_df)
            if prediction is not None:
                last_time = len(usage_df_with_time) - 1
                fig_pred.add_trace(go.Scatter(
                    x=[last_time, last_time + 1],
                    y=[usage_df_with_time[core_cols[-1]].iloc[-1] if len(usage_df_with_time) > 0 else 0, prediction],
                    mode='lines+markers',
                    name='AI Prediction',
                    line=dict(color=COLORS['warning'], dash='dash', width=3),
                    marker=dict(size=10)
                ))
            
            fig_pred.update_layout(
                title="Core Utilization Over Time with AI Prediction",
                xaxis_title="Time Step",
                yaxis_title="Utilization (%)",
                yaxis_range=[0, 100],
                template='plotly_dark',
                height=400
            )
        else:
            fig_pred = go.Figure()
            fig_pred.add_annotation(text="Insufficient data for prediction", showarrow=False)
            fig_pred.update_layout(template='plotly_dark', height=400)
    else:
        fig_pred = go.Figure()
        fig_pred.add_annotation(text="No usage data available", showarrow=False)
        fig_pred.update_layout(template='plotly_dark', height=400)
    
    return energy_display, status_text, fig_util, core_panels, table, migrations_display, fig_pred

if __name__ == '__main__':
    # Ensure data directory exists
    os.makedirs('data', exist_ok=True)
    
    # Create sample stats.json if it doesn't exist
    if not os.path.exists('data/stats.json'):
        sample_stats = {
            "timestamp": datetime.now().isoformat(),
            "energy_mode": "Balanced",
            "cores": [
                {"id": 0, "utilization": 35.0, "tasks": [{"id": 1, "priority": 3}]},
                {"id": 1, "utilization": 42.0, "tasks": [{"id": 2, "priority": 5}]},
                {"id": 2, "utilization": 25.0, "tasks": [{"id": 3, "priority": 1}]},
                {"id": 3, "utilization": 50.0, "tasks": []}
            ],
            "tasks": [
                {"id": 1, "priority": 3, "deadline": 10.0, "core_assigned": 0, "completed": False, "cpu_time": 0.0},
                {"id": 2, "priority": 5, "deadline": 15.0, "core_assigned": 1, "completed": False, "cpu_time": 0.0},
                {"id": 3, "priority": 1, "deadline": 5.0, "core_assigned": 2, "completed": False, "cpu_time": 0.0}
            ],
            "migrations": []
        }
        with open('data/stats.json', 'w') as f:
            json.dump(sample_stats, f, indent=2)
        print("Created sample stats.json file")
    
    print("Starting CoreManager 2.0 Dashboard...")
    print("Open http://127.0.0.1:8050 in your browser")
    app.run_server(debug=True, host='127.0.0.1', port=8050)

