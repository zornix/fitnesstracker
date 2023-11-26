import pandas as pd
import matplotlib.pyplot as plt

# Load the exported CSV data into a pandas DataFrame
data = pd.read_csv('1RM_data.csv')

# Convert the 'Date' column in the DataFrame to datetime type 
data['Date'] = pd.to_datetime(data['Date'])

plt.figure(figsize=(10, 6))

# Plotting the data points using date on the x-axis and 1RM values on the y-axis
plt.plot(data['Date'], data['1RM'], marker='o', linestyle='-')

# Labels
plt.title('1RM Progress over Time')
plt.xlabel('Date')
plt.ylabel('1RM (lbs)')

plt.xticks(rotation=45) # Rotating x-axis labels
plt.tight_layout() 
plt.show()
