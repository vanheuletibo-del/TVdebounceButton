#ifndef DEBOUNCED_BUTTON_H                                                                              // If the DEBOUNCED_BUTTON_H macro is not defined
    #define DEBOUNCED_BUTTON_H                                                                          // Define the DEBOUNCED_BUTTON_H macro to prevent multiple inclusions of this header file

    enum toestanden {                                                                                   // Enumeration to represent the different states of the button  
        BS_INITIALISED,                                                                                 // Initial state of the button
        BS_WAITING,                                                                                     // Waiting for button press state              
        BS_BUTTON_DOWN,                                                                                 // Button pressed down state
        BS_BUTTON_UP,                                                                                   // Button released state
        BS_CLICK,                                                                                       // Button click state   
        BS_DOUBLE_CLICK                                                                                 // Button double click state    
    };

    class DebouncedButton {                                                                             // Class definition for the DebouncedButton class
        private:                                                                                        // Private member variables and functions of the DebouncedButton class
            int _pin;                                                                                   // Pin number for the button
            int _debounceDelay;                                                                         // Debounce delay in milliseconds
            int _ClickDelay;                                                                            // Click delay in milliseconds
            int _dbClickDelay;                                                                          // Double click delay in milliseconds                            
            enum toestanden _toestand;                                                                  // Current state of the button
            bool _pullUP;                                                                               // Use internal pull-up resistor if true, otherwise use external pull-up resistor
            bool _secondClick;                                                                          // Flag to track if a second click has occurred                          

        void (*_onButtonDown)(void);                                                                    // Callback function for button down event
        void (*_onButtonUp)(void);                                                                      // Callback function for button up event
        void (*_onButtonClick)(void);                                                                   // Callback function for button click event
        void (*_onButtonDoubleClick)(void);                                                             // Callback function for button double click event

        unsigned long _ontdenderVertragingVerstrekenOm;                                                 // Debounce delay timer for button down event
        unsigned long _ClickVertragingVerstrekenOm;                                                     // Click delay timer for button down event   
        unsigned long _dbClickVertragingVerstrekenOm;                                                   // Double click delay timer for button down event
        
        int _vorigeStatus;                                                                              // Variable to store the previous status of the button pin


        public:                                                                                         // Public member functions of the DebouncedButton class 
            DebouncedButton(int pin, int debounceDelay,                                                 // Constructor for the DebouncedButton class with parameters for pin number, debounce delay, click delay, double click delay, and pull-up resistor usage
                 int ClickDelay = 500, int dbClickDelay = 100, bool pullUp = true);                     // Constructor for the DebouncedButton class with parameters for pin number, debounce delay, click delay, double click delay, and pull-up resistor usage
    
            void onButtonUp(void (*callback)());                                                        // Register a callback function for the button up event
            void onButtonClick(void (*callback)());                                                     // Register a callback function for the button click event
            void onButtonDoubleClick(void (*callback)());                                               // Register a callback function for the button double click event                           
            void onButtonDown(void (*callback)());                                                      // Register a callback function for the button down event


            void begin(void);                                                                           // Initialize the button by setting the pin mode and initial state
            int update(void);                                                                           // Update the button state and handle events based on the current state and button input
    };

#endif                                                                                                  // End of the DEBOUNCED_BUTTON_H macro definition