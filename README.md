# Interprocess Communication (IPC) in C

This repository is dedicated to exploring different Interprocess Communication (IPC) mechanisms using C programming language. It is structured into three main branches, each focusing on a distinct IPC method.

## Branches Overview

### 1. Main Branch: Message Queues
The `main` branch provides a detailed C implementation of message queues from scratch. It includes examples of both server and client components that communicate via a message queue system. This branch is particularly useful for understanding the basics of message queue operations in C.

#### Compilation Commands for Message Queues
- **Compile the server**: `make servidor.out && ./servidor.out`
- **Compile the client**: `make cliente.out && ./cliente.out`
  - Note: The `-lrt` flag is used to enable real-time functions.

- **View the message queue**: `ls /dev/mqueue/`
- **Read the content of the message queue**: `cat /dev/mqueue/<queue_name>`
- **Delete a message queue**: `rm /dev/mqueue/<queue_name>`
  - For example: `rm /dev/mqueue/mq_server` or `rm /dev/mqueue/mq_client_0`

#### File Structure

![File Structure](https://github.com/aaronespasa/interprocess-communications-in-c/blob/main/message-queues.png)

### 2. Sockets Branch
The `sockets` branch contains a C implementation of IPC using the sockets library. This implementation provides a practical example of how processes can communicate over a network, using sockets as the communication endpoint.

#### File Structure

![File Structure](https://github.com/aaronespasa/interprocess-communications-in-c/blob/main/sockets.png)

### 3. RPC Branch
On the `rpc` branch, you'll find a C implementation of Remote Procedure Call (RPC) using ONC RPC. This branch is ideal for understanding how distributed applications can perform procedure calls over a network, abstracting the network communication details.

#### File Structure

![File Structure](https://github.com/aaronespasa/interprocess-communications-in-c/blob/main/rpc.png)

## Getting Started
To get started, clone the repository and switch to the branch that corresponds to the IPC method you're interested in exploring:

```bash
git clone https://github.com/aaronespasa/interprocess-communications-in-c.git
git checkout [branch_name]
```
