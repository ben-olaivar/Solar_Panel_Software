This version is designed as a call and response program. The head node first sends a message to the desired node, then waits for a response
from said node.

PROS: 
- Easy integration for commands on both head and nodes
- 

CONS: 
- (In current state 7/19/2021) it's difficult to add new nodes if code is already running. Can be fixed by resetting head node
- There is a breif moment where the node may not receive the message, and both the node and head are waiting for a message.
This could happen literally at any time