import pickle

# 
# @Deserialization
# 
# Deserialization is the process of converting data from a serialized format back into its original object form in memory. 
# This process is the inverse of serialization, which involves converting an object into a format that can be easily stored or transmitted.
#

"""
Example of deserialization:
    import pickle

    # Read the serialized data from the file
    with open('data.pkl', 'rb') as file:
        serialized_data = file.read()

    # Deserialize the data back to the original object
    data = pickle.loads(serialized_data)

    print(data)  # Output: {'name': 'Alice', 'age': 30, 'is_student': False}
"""

# Function to load user-provided data
def load_user_data(serialized_data):
    """
    Function to load user-provided data. The deserialization process
    is vulnerable to a pickle deserialization attack.
    
    Args:
        serialized_data: Pickled data to deserialize
    
    Returns:
        The deserialized object
    """
    # Deserializing user-provided data (DANGEROUS)
    # The `pickle.loads()` function deserializes the byte stream into a Python object.
    # This is dangerous because `pickle` can execute arbitrary code during deserialization.
    data = pickle.loads(serialized_data)
    return data

# Read pickled data from a file
if __name__ == "__main__":
    # Specify the path to the file containing the pickled data
    file_path = "rick.txt"
    
    try:
        # Open the file in binary mode and read its content
        with open(file_path, 'rb') as file:
            # Read the entire content of the file
            binary_data = file.read()
            
            # Directly pass the binary data to `load_user_data()`
            user_data = load_user_data(binary_data)
            
            # Print the deserialized data
            print("Deserialized Data:", user_data)
    
    except Exception as e:
        # Handle exceptions that may occur during file reading or deserialization
        print(f"Failed to deserialize data: {e}")
