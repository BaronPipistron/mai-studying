#include <bits/stdc++.h>
#include "zmq.h"

const int DEFAULT_PORT = 5050;
int n = 2;

bool send_message(void* socket, const std::string& message_string) 
{
   int rc;
   zmq_msg_t msg;
   rc = zmq_msg_init_size(&msg, message_string.size());

   if (rc != 0) {
      std::cerr << "SEND_MESSAGE ERROR: Error while initializing message: " << zmq_strerror(errno)
                << std::endl;
      return false;
   }

   memcpy(zmq_msg_data(&msg), message_string.c_str(), message_string.size());
   rc = zmq_msg_send(&msg, socket, 0);

   if (rc == -1) {
      std::cerr << "SEND_MESSAGE ERROR: Error while sending message: " << zmq_strerror(errno)
                << std::endl;
      zmq_msg_close(&msg);
      return false;
   }

   zmq_msg_close(&msg);
   return true;
}

std::string receive_message(void* socket) 
{
   int rc;
   zmq_msg_t msg;
   rc = zmq_msg_init(&msg);

   if (rc != 0) {
      std::cerr << "RECEIVE_MESSAGE ERROR: Error while initializing message: " << zmq_strerror(errno)
                << std::endl;
      return "";
   }

   rc = zmq_msg_recv(&msg, socket, 0);
   if (rc == -1) {
      std::cerr << "RECEIVE_MESSAGE ERROR: Error while recieving message: " << zmq_strerror(errno)
                << std::endl;
      zmq_msg_close(&msg);
      return "";
   }

   std::string received_message(static_cast<char*>(zmq_msg_data(&msg)),
                                zmq_msg_size(&msg));

   zmq_msg_close(&msg);
   return received_message;
}

void create_node(const int& id, const int& port) 
{
   char* arg0 = strdup("./node");
   char* arg1 = strdup((std::to_string(id)).c_str());
   char* arg2 = strdup((std::to_string(port)).c_str());
   char* args[] = {arg0, arg1, arg2, nullptr};

   execv("./node", args);
}

std::string get_port_name(const int& port) 
{
   return "tcp://127.0.0.1:" + std::to_string(port);
}

bool is_number(const std::string& val) 
{
   try {
      int tmp = stoi(val);
      return true;
   } catch (std::exception& ex) {
      std::cout << "IS_NUMBER ERROR: " << ex.what() << std::endl;
      return false;
   }
}

int main() {
   std::string command;

   int root_id = 0;
   int root_pid = 0;
   void* context = zmq_ctx_new();
   void* root_socket = zmq_socket(context, ZMQ_REQ);

   std::cout << "Commands:" << std::endl;
   std::cout << "1. create (id)" << std::endl;
   std::cout << "2. exec (id) (numbers_of_nums, k_1...k_n)" << std::endl;
   std::cout << "3. kill (id)" << std::endl;
   std::cout << "4. pingall" << std::endl;
   std::cout << "5. exit" << std::endl << std::endl;

   std::vector<int> node_ids;

   while (true) {
      std::cin >> command;
      int node_id = 0;

      std::string id_str = "";
      std::string reply = "";

      if (command == "create") {
         ++n;
         std::cin >> id_str;

         if (!is_number(id_str)) {
            continue;
         }

         node_id = stoi(id_str);
         node_ids.push_back(node_id);

         if (root_pid == 0) {
            zmq_bind(root_socket,
                     get_port_name(DEFAULT_PORT + node_id).c_str());

            zmq_setsockopt(root_socket, ZMQ_RCVTIMEO, NULL, n * 500);
            zmq_setsockopt(root_socket, ZMQ_SNDTIMEO, NULL, n * 500);

            root_pid = fork();
            if (root_pid == -1) {
               std::cout << "CREATE ERROR: Unable to create first worker node\n";
               root_pid = 0;
               exit(1);
            } else if (root_pid == 0) {
               create_node(node_id, DEFAULT_PORT + node_id);
            } else {
               root_id = node_id;
               send_message(root_socket, "pid");
               reply = receive_message(root_socket);
            }
         } else {
            zmq_setsockopt(root_socket, ZMQ_RCVTIMEO, NULL, n * 500);
            zmq_setsockopt(root_socket, ZMQ_SNDTIMEO, NULL, n * 500);
            std::string request = "create " + std::to_string(node_id);
            send_message(root_socket, request);
            reply = receive_message(root_socket);
         }

         std::cout << reply << std::endl;
      }

      if (command == "kill") {
         std::cin >> id_str;

         if (root_pid == 0) {
            std::cout << "Root is dead!" << std::endl;
            continue;
         }
         if (!is_number(id_str)) {
            continue;
         }

         node_id = stoi(id_str);
         if (node_id == root_id) {
            kill(root_pid, SIGKILL);
            root_id = 0;
            root_pid = 0;
            std::cout << "Ok\n";
            continue;
         }

         std::string request = "kill " + std::to_string(node_id);
         send_message(root_socket, request);
         reply = receive_message(root_socket);
         std::cout << reply << std::endl;
      }

      if (command == "exec") {
         int number_of_nums = 0;
         std::string nums_str = "";
         std::cin >> id_str >> number_of_nums;

         for (size_t i = 0; i != number_of_nums; ++i) {
            int num;
            std::cin >> num;
            nums_str += (std::to_string(num) + "_");
         }
         nums_str.pop_back();

         if (root_pid == 0) {
            std::cout << "Root is dead!" << std::endl;
            continue;
         }
         if (!is_number(id_str)) {
            continue;
         }

         node_id = stoi(id_str);
         std::string request = "exec " + std::to_string(node_id) + " " + nums_str;
         send_message(root_socket, request);
         reply = receive_message(root_socket);
         std::cout << reply << std::endl;
      }

      if (command == "pingall") {
         if (root_pid == 0) {
            std::cout << "Root is dead!" << std::endl;
            continue;
         }

         std::string reply_1 = "";

         for (size_t i = 0; i != node_ids.size(); ++i) {
            if (!is_number(std::to_string(node_ids[i]))) {
               continue;
            }

            std::string request = "ping " + std::to_string(node_ids[i]);
            send_message(root_socket, request);
            reply_1 = receive_message(root_socket);

            if (reply_1 == "Ok: 1") {
               reply += std::to_string(node_ids[i]) + ';';
            } else {
               continue;
            }
         }

         if (reply.empty()) {
            reply = "Ok: -1";
         } else {
            reply = "Ok: " + reply;
         }
         reply.pop_back();

         std::cout << reply << std::endl;
      }

      if (command == "exit") {
         int t = system("killall -9 node");
         break;
      }
   }

   zmq_close(root_socket);
   zmq_ctx_destroy(context);

   return 0;
}
