# include<iostream> 
# include<windows.h> 
using namespace std;
#pragma comment(lib, "ws2_32.lib")
HINSTANCE hinstance;



string server_ip_adress = "192.000.00.000";
string server_port_number = "1234";


SOCKADDR_IN serverAddress;
SOCKET clientSocket;




HWND hcreate_connect_button, hcreate_id_textfield, hcreate_password_textfield,
hcreate_login_button, hcreate_signup_button, hcreateaccount_button, hcreate_signuppassword, hcreate_signupid;

WNDCLASS wccreate_main_window, wccreate_loginsignup_window, wcsignup_window, wc_homepage;
HWND hcreate_main_window, hcreate_loginsignup_window, hsignup_window, hcreate_homepage;


 const int  connect_button_id = 1, id_textfield_id = 2, password_textfield_id = 3, login_button_id = 4, signup_button_id = 5, signupid_id = 6, signuppassword_id = 7, create_account_id = 8;


LRESULT CALLBACK f_create_main_window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK f_create_loginsignup_window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK f_create_homepage_Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK f_create_signup_window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


void f_create_main_window();
void f_create_loginsignup_window();
void f_create_signup_window();
void f_create_homepage();


void f_create_connect_button();
void f_handle_login_button();
void f_handle_signup_button();
void f_handle_createaccount_button();

void send_to_server(string);
void recieve(string&);
int main() {

    hinstance = GetModuleHandle(NULL);// handle of the programm is stored in the hinstance 
    f_create_main_window();

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);

}
void f_create_main_window() {

    // Register the window class
    wccreate_main_window = {};
    //  procedue function of this window 
    wccreate_main_window.lpfnWndProc = f_create_main_window_proc;
    wccreate_main_window.hInstance = hinstance;
    wccreate_main_window.lpszClassName = L"MainWindowClass";
    RegisterClass(&wccreate_main_window);

    // Create the main window
    hcreate_main_window = CreateWindow(
        wccreate_main_window.lpszClassName,
        L"Main Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, wccreate_main_window.hInstance, NULL
    );

    // Create the "Connect" button
    f_create_connect_button();

    // Show and update the main window
    ShowWindow(hcreate_main_window, SW_SHOW);
    UpdateWindow(hcreate_main_window);


}
void f_create_connect_button() {

    // i  am storing the handle of button in  hcreate_connect_button  hcreate_main_window is the handle of its parent window 
    // also you have to constant value to function which is id of button   connect_button_id


    hcreate_connect_button = CreateWindow(
        L"BUTTON",
        L"Connect",
        WS_VISIBLE | WS_CHILD,
        150, 120, 80, 30,
        hcreate_main_window,
        reinterpret_cast<HMENU>(static_cast<UINT_PTR>(connect_button_id)),
        GetModuleHandle(NULL),
        NULL
    );

}

void  f_create_loginsignup_window() {
    WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        std::cout << "Failed to initialize winsock" << std::endl;
        MessageBox(NULL, L"FAILED TO CONNECT!", L"Message", MB_OK);
        return;
    }

    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "Failed to create socket" << std::endl;
        MessageBox(NULL, L"FAILED TO CONNECT!", L"Message", MB_OK);
        WSACleanup();
        return;
    }

    // Set up the server address

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);  // Change the port number if needed
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");  // Change the IP address if needed

    // Connect to the server
    if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cout << "Failed to connect to server" << std::endl;
        MessageBox(NULL, L"FAILED TO CONNECT!", L"Message", MB_OK);
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    // Hide the main window
    ShowWindow(hcreate_main_window, SW_HIDE);
    // register window class 
    wccreate_loginsignup_window = {};
    //  procedue function of this window 
    wccreate_loginsignup_window.lpfnWndProc = f_create_loginsignup_window_proc;
    wccreate_loginsignup_window.hInstance = hinstance;
    wccreate_loginsignup_window.lpszClassName = L"login/signupClass";
    RegisterClass(&wccreate_loginsignup_window);

    // Create the login/signup window
    hcreate_loginsignup_window = CreateWindow(
        wccreate_loginsignup_window.lpszClassName,
        L"Login/Signup Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, GetModuleHandle(NULL), NULL
    );

    // Create the ID text field
    hcreate_id_textfield = CreateWindow(
        L"EDIT",
        NULL,
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        150, 20, 150, 30,
        hcreate_loginsignup_window,
        reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id_textfield_id)),
        GetModuleHandle(NULL),
        NULL
    );

    // Create the password text field
    hcreate_password_textfield = CreateWindow(
        L"EDIT",
        NULL,
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
        150, 70, 150, 30,
        hcreate_loginsignup_window,
        reinterpret_cast<HMENU>(static_cast<UINT_PTR>(password_textfield_id)),
        GetModuleHandle(NULL),
        NULL
    );

    // Create the "Login" button
    hcreate_login_button = CreateWindow(
        L"BUTTON",
        L"Login",
        WS_VISIBLE | WS_CHILD,
        100, 120, 80, 30,
        hcreate_loginsignup_window,
        reinterpret_cast<HMENU>(static_cast<UINT_PTR>(login_button_id)),
        GetModuleHandle(NULL),
        NULL
    );

    // Create the "Signup" button
    hcreate_signup_button = CreateWindow(
        L"BUTTON",
        L"Signup",
        WS_VISIBLE | WS_CHILD,
        200, 120, 80, 30,
        hcreate_loginsignup_window,
        reinterpret_cast<HMENU>(static_cast<UINT_PTR>(signup_button_id)),
        GetModuleHandle(NULL),
        NULL
    );

    // Show and update the login/signup window
    ShowWindow(hcreate_loginsignup_window, SW_SHOW);
    UpdateWindow(hcreate_loginsignup_window);
}
LRESULT CALLBACK f_create_loginsignup_window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg) {
    case WM_COMMAND:
        if (LOWORD(wParam) == login_button_id) {
            f_handle_login_button();
        }
        else if (LOWORD(wParam) == signup_button_id) {
            f_handle_signup_button();
        }

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;


}
void f_handle_signup_button() {
    cout << "signup button clicked" << endl;
    f_create_signup_window();
    

}
void f_handle_createaccount_button() {
    cout << "create account button clicked" << endl;
    string s1, s2;
    char buffer1[256], buffer2[256];
    GetWindowTextA(hcreate_signupid,buffer1,sizeof(buffer1));
    GetWindowTextA(hcreate_signuppassword,buffer2,sizeof(buffer2));
    s1 = buffer1; s2 = buffer2;
    cout << s1 << s2 << endl;
    string signupdata = "2," + s1 + "," + s2+",";
    send_to_server(signupdata);
    string recieved;
    recieve(recieved);
    // MessageBox(NULL,L"account created", L"RESPONSE", MB_OK | MB_ICONINFORMATION);

    if (recieved == "account created") {
        // send_to_server(go_back_to_start_page);
        MessageBox(NULL, L"account created", L"RESPONSE", MB_OK | MB_ICONINFORMATION);
    }
    else {
        //send_to_server(go_back_to_start_page);
        MessageBox(NULL, L"such user name already exist", L"RESPONSE", MB_OK | MB_ICONINFORMATION);
    }
}
void f_create_signup_window() {
    ShowWindow(hcreate_loginsignup_window, SW_HIDE);
    cout << "signup window creating " << endl;
    wcsignup_window = {};
    //  procedue function of this window 
    wcsignup_window.lpfnWndProc = f_create_signup_window_proc;
    wcsignup_window.hInstance = hinstance;
    wcsignup_window.lpszClassName = L"signupclass";
    RegisterClass(&wcsignup_window);

    // Create the login/signup window
    hsignup_window = CreateWindow(
        wcsignup_window.lpszClassName,
        L"signup window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, GetModuleHandle(NULL), NULL
    );

    // Create the ID text field
    hcreate_signupid = CreateWindow(
        L"EDIT",
        NULL,
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        150, 20, 150, 30,
        hsignup_window,
        reinterpret_cast<HMENU>(static_cast<UINT_PTR>(signupid_id)),
        GetModuleHandle(NULL),
        NULL
    );

    // Create the password text field
    hcreate_signuppassword = CreateWindow(
        L"EDIT",
        NULL,
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
        150, 70, 150, 30,
        hsignup_window,
        reinterpret_cast<HMENU>(static_cast<UINT_PTR>(signuppassword_id)),
        GetModuleHandle(NULL),
        NULL
    );

    // Create the "create account" button
    hcreateaccount_button = CreateWindow(
        L"BUTTON",
        L"create account",
        WS_VISIBLE | WS_CHILD,
        100, 120, 130, 30,
        hsignup_window,
        reinterpret_cast<HMENU>(static_cast<UINT_PTR>(create_account_id)),
        GetModuleHandle(NULL),
        NULL
    );
    cout << "signup window created " << endl;

    // Show and update the login/signup window
    ShowWindow(hsignup_window, SW_SHOW);
    UpdateWindow(hsignup_window);

}
LRESULT CALLBACK f_create_signup_window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
   // if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == 1) {
        // Print button clicked.
   //     char buffer[256];
   //     GetWindowTextA(textField, buffer, sizeof(buffer));
   //     userInput = buffer;

   //     std::cout << "User Input: " << userInput << std::endl;
   // }
   //
    switch (msg)
    {
    case WM_COMMAND:
        cout << "inside the command section of signup window" << endl;
        if (LOWORD(wParam) == create_account_id&& HIWORD(wParam) == BN_CLICKED)
            f_handle_createaccount_button();
        
        break;
    case WM_DESTROY:
        send_to_server("5");
        ShowWindow(hcreate_loginsignup_window, SW_SHOW);
        DestroyWindow(hwnd);
        //PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);



    }
    return 0;
}
   void f_handle_login_button() {
    string s1, s2;
    char buffer3[256], buffer4[256];
    GetWindowTextA(hcreate_id_textfield,buffer3,sizeof(buffer3));
    GetWindowTextA(hcreate_password_textfield,buffer4,sizeof(buffer4));
    s1 = buffer3; s2 = buffer4;
    string log_in_data = "1," + s1 + "," + s2+",";
    cout << log_in_data << endl;
    send_to_server(log_in_data);


    string recieved;
    recieve(recieved);
    cout << recieved << endl;

    if (recieved == "error") {
        MessageBox(NULL, L"error", L"RESPONSE", MB_OK | MB_ICONINFORMATION);
        
    }
    else {
        f_create_homepage();

    }


}
void f_create_homepage() {
    ShowWindow(hcreate_loginsignup_window, SW_HIDE);
    wc_homepage = { 0 };
    //  procedue function of this window 
    wc_homepage.lpfnWndProc = f_create_homepage_Proc;
    wc_homepage.hInstance = hinstance;
    wc_homepage.lpszClassName = L"HomePageClass";
    RegisterClass(&wc_homepage);

    // Create the main window
    hcreate_homepage = CreateWindow(
        wc_homepage.lpszClassName,
        L"HOME PAGE",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, wc_homepage.hInstance, NULL
    );



    // Show and update the main window
    ShowWindow(hcreate_homepage, SW_SHOW);
    //UpdateWindow(hcreate_homepage);

}
LRESULT CALLBACK f_create_homepage_Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {

    case  WM_COMMAND:


        break;

    case  WM_DESTROY:
        send_to_server("logout");
        ShowWindow(hcreate_loginsignup_window, SW_SHOW);
        DestroyWindow(hwnd);
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);

    }
    return 0;
}
void recieve(string& s) {
    char buffer[1024];
    std::string serverResponse;
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';  // Null-terminate the received data
        serverResponse = buffer;
        std::cout << "Server response: " << serverResponse << std::endl;
    }
    else if (bytesRead == 0) {
        std::cerr << "Server closed the connection.\n";
    }
    else {
        std::cerr << "Failed to receive response from the server.\n";
    }
    s = serverResponse;
    return;

}
void send_to_server(string message) {

     int length = message.length();

    // declaring character array (+1 for null terminator)
    char* char_array = new char[length + 1];

    // copying the contents of the
    // string to char array
    

    if (send(clientSocket,message.c_str(), length, 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message to the server.\n";
        closesocket(clientSocket);
        WSACleanup();
        return;
    }
    return;

}
LRESULT CALLBACK f_create_main_window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_COMMAND:
        if (LOWORD(wParam) == connect_button_id) {
            f_create_loginsignup_window();
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}
