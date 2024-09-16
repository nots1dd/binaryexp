import pickle

# Original data
data = {'name': 'Alice', 'age': 30, 'is_student': False}

# Serialize data to a byte stream
serialized_data = pickle.dumps(data)

# Save the serialized data to a file
with open('actualrick.txt', 'wb') as file:
    file.write(serialized_data)

"""
@WHY IS THIS USED 

Data Storage

    Serialization: Converts complex data structures or objects into a format that can be easily saved to a file or database. 
    This might include converting objects to JSON, XML, or binary formats.

    Deserialization: Converts the stored data back into a usable object or data structure. 
    This allows programs to reconstruct and use the data that was previously saved.

Data Transmission

    Serialization: Converts data into a format suitable for transmission over networks. 
    This could be JSON or XML for web services, or binary formats for more efficient data transfer.

    Deserialization: Converts the transmitted data back into its original format for use by the receiving application.
"""
