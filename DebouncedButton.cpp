#include <Arduino.h>                                                                                                    // Arduino core library
#include <DebouncedButton.h>                                                                                            // DebouncedButton library header

 

    
DebouncedButton::DebouncedButton(int pin, int debounceDelay, int ClickDelay, int dbClickDelay, bool pullUP) {           // Constructor to initialize the DebouncedButton object with specified parameters
    _pin                    =pin;                                                                                       // Pin number for the button
    _debounceDelay          = debounceDelay;                                                                            // Debounce delay in milliseconds
    _ClickDelay             = ClickDelay;                                                                               // Click delay in milliseconds
    _dbClickDelay           = dbClickDelay;                                                                             // Double click delay in milliseconds
    _pullUP                 = pullUP;                                                                                   // Use internal pull-up resistor if true, otherwise use external pull-up resistor
    _secondClick             = false;                                                                                   // Flag to track if a second click has occurred

    _onButtonClick          = nullptr;                                                                                  // Callback function for button click event
    _onButtonDoubleClick    = nullptr;                                                                                  // Callback function for button double click event
    _onButtonDown           = nullptr;                                                                                  // Callback function for button down event
    _onButtonUp             = nullptr;                                                                                  // Callback function for button up event

}

void DebouncedButton::begin(void){                                                                                      // Initialize the button by setting the pin mode and initial state
    _toestand = BS_INITIALISED;                                                                                         // Set the initial state of the button to BS_INITIALISED

    if(_pullUP)                                                                                                         // If using internal pull-up resistor, set the pin mode to INPUT_PULLUP
        pinMode(_pin, INPUT_PULLUP);                                                                                    // Otherwise, set the pin mode to INPUT
    else                                                                                                                // Set the pin mode to INPUT
        pinMode(_pin, INPUT);                                                                                           // Set the pin mode to INPUT
}
 void DebouncedButton::onButtonDown(void (*callback)()) {                                                               // Register a callback function for the button down event
    _onButtonDown = callback;                                                                                           // Store the callback function pointer for button down event
}
void DebouncedButton::onButtonUp(void (*callback)()) {                                                                  // Register a callback function for the button up event
    _onButtonUp = callback;                                                                                             // Store the callback function pointer for button up event            
}
void DebouncedButton::onButtonClick(void (*callback)()) {                                                               // Register a callback function for the button click event
    _onButtonClick = callback;                                                                                          // Store the callback function pointer for button click event   
}
void DebouncedButton::onButtonDoubleClick(void (*callback)()) {                                                         // Register a callback function for the button double click event
    _onButtonDoubleClick = callback;                                                                                    // Store the callback function pointer for button double click event
}

int DebouncedButton::update(void){                                                                                      // Update the button state and handle events based on the current state and button input
    unsigned long nu = millis();                                                                                        // Get the current time in milliseconds since the program started
    int huidigeStatus = digitalRead(_pin);                                                                              // Read the current status of the button pin (0 for LOW, 1 for HIGH)

    switch (_toestand){                                                                                                 // Handle different states of the button based on the current state and button input
        case BS_INITIALISED:                                                                                            // If the button is in the initial state, transition to the waiting state
            _toestand = BS_WAITING;                                                                                     // Set the state to BS_WAITING
            break;                                                                                                      // If the button is in the waiting state, check for button press events and handle them accordingly
        case BS_WAITING:                                                                                                // If the button is pressed (huidigeStatus == 0), trigger the button down event and transition to the BS_BUTTON_DOWN state
            if (huidigeStatus == 0) {                                                                                   // If the button is pressed (huidigeStatus == 0), trigger the button down event and transition to the BS_BUTTON_DOWN state
                if (_onButtonDown != nullptr) _onButtonDown();                                                          // Trigger the button down event if a callback function is registered
                _toestand = BS_BUTTON_DOWN;                                                                             // Transition to the BS_BUTTON_DOWN state
                _ontdenderVertragingVerstrekenOm = nu + _debounceDelay;                                                 // Set the debounce delay timer for the button down event
                _ClickVertragingVerstrekenOm = nu + _ClickDelay;                                                        // Set the click delay timer for the button down event
            } 
            break;                                                                                                      // If the button is released (huidigeStatus == 1) and the debounce delay has elapsed, trigger the button up event and handle click/double click events accordingly

        case BS_BUTTON_DOWN:                                                                                            // If the button is released (huidigeStatus == 1) and the debounce delay has elapsed, trigger the button up event and handle click/double click events accordingly           
            if ((huidigeStatus == 1) && (nu > _ontdenderVertragingVerstrekenOm)) {                                      // If the button is released (huidigeStatus == 1) and the debounce delay has elapsed, trigger the button up event and handle click/double click events accordingly
                if (_onButtonUp != nullptr) _onButtonUp();                                                              // Trigger the button up event if a callback function is registered
                if (_secondClick) {                                                                                     // If a second click has occurred, trigger the button double click event and transition to the BS_WAITING state
                    if (_onButtonDoubleClick != nullptr) _onButtonDoubleClick();                                        // Trigger the button double click event if a callback function is registered
                    _secondClick = false;                                                                               // Reset the second click flag
                    _toestand = BS_WAITING;                                                                             // Transition to the BS_WAITING state
                } else {                                                                                                // If a second click has not occurred, transition to the BS_CLICK state and set the click delay timer                          
                    _toestand = BS_CLICK;                                                                               // Transition to the BS_CLICK state
                    _ClickVertragingVerstrekenOm = nu + _dbClickDelay;                                                  // Set the double click delay timer for the button click event
                }
            }
            break;                                                                                                      // If the button is pressed again (huidigeStatus == 0) while in the BS_CLICK state, trigger the button down event and transition to the BS_BUTTON_DOWN state
        case BS_CLICK:                                                                                                  // If the button is pressed again (huidigeStatus == 0) while in the BS_CLICK state, trigger the button down event and transition to the BS_BUTTON_DOWN state                                    
            if (huidigeStatus == 0) {                                                                                   // If the button is pressed again (huidigeStatus == 0) while in the BS_CLICK state, trigger the button down event and transition to the BS_BUTTON_DOWN state
                if (_onButtonDown != nullptr) _onButtonDown();                                                          // Trigger the button down event if a callback function is registered
                _secondClick = true;                                                                                    // Set the second click flag to true                         
                _toestand = BS_BUTTON_DOWN;                                                                             // Transition to the BS_BUTTON_DOWN state
                _ontdenderVertragingVerstrekenOm = nu + _debounceDelay;                                                 // Set the debounce delay timer for the button down event
            } else if (nu > _ClickVertragingVerstrekenOm) {                                                             // If the click delay has elapsed without a second click, trigger the button click event and transition to the BS_WAITING state
                if (_onButtonClick != nullptr) _onButtonClick();                                                        // Trigger the button click event if a callback function is registered
                _toestand = BS_WAITING;                                                                                 // Transition to the BS_WAITING state
            }
            break;                                                                                                      // If the button is released (huidigeStatus == 1) and the debounce delay has elapsed, trigger the button up event and transition to the BS_WAITING state
        case BS_DOUBLE_CLICK:                                                                                           // If the button is released (huidigeStatus == 1) and the debounce delay has elapsed, trigger the button up event and transition to the BS_WAITING state
            _toestand = BS_WAITING;                                                                                     // Transition to the BS_WAITING state
            break;                                                                                                      // Default case to handle any unexpected states and transition to the BS_WAITING state
        default:                                                                                                        // Default case to handle any unexpected states and transition to the BS_WAITING state
            _toestand = BS_WAITING;                                                                                     // Transition to the BS_WAITING state   
            break;                                                                                                      // Default case to handle any unexpected states and transition to the BS_WAITING state
    }
    /*
    if ((_vorigeStatus != huidigeStatus) && (nu > _ontdenderVertragingVerstrekenOm)) {
        _ontdenderVertragingVerstrekenOm = nu + _debounceDelay;
        _vorigeStatus = huidigeStatus;
    }
    */
    return 0;                                                                                                            // Return 0 to indicate successful update of the button state
}
