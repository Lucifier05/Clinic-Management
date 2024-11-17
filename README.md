# Clinic Management System

A C++ application for managing patient and doctor interactions in a medical clinic. The system handles doctor check-ins/outs, patient assignments, emergency cases, and waiting list management.

## Features

- **Doctor Management**
  - Check-in/out system
  - Room assignment
  - Specialty tracking
  - Patient waiting list management

- **Patient Management**
  - Check-in/out system
  - Emergency case prioritization
  - Age-based routing (pediatric cases)
  - Automatic doctor assignment based on specialty

- **Smart Assignment System**
  - Matches patients with appropriate specialists
  - Falls back to general practitioners when specialists unavailable
  - Manages emergency cases with priority queuing
  - Automatic patient reassignment when doctors check out

- **Room Management**
  - Tracks room availability
  - Supports up to 100 rooms
  - Prevents double booking

## Technical Details

### Class Structure

- `Clinic`: Main system controller
- `Doctor`: Manages doctor information and waiting lists
- `Patient`: Stores patient information
- `Room`: Handles room assignment and availability

### Dependencies

- C++ Standard Library
- Standard Template Library (STL)
- File I/O streams for logging

## Getting Started

### Prerequisites

- C++ compiler with C++11 support or later
- Make or similar build system

### Building the Project

```bash
g++ -o clinic main.cpp clinic.cpp -std=c++11
```

### Running the Application

```bash
./clinic
```

## Usage

### Doctor Operations

```
D I - Doctor Check-in
  - Enter name
  - Enter room number (1-100)
  - Enter specialty code

D O - Doctor Check-out
  - Enter name
```

### Patient Operations

```
P I - Patient Check-in
  - Enter name
  - Enter age
  - Enter required specialty
  - Specify if emergency (1=yes, 0=no)

P O - Patient Check-out
  - Enter name
```

### Specialty Codes

- `PED` - Pediatrics (automatically assigned for patients under 16)
- `GEN`/`GP` - General Practice
- Other specialty codes as defined by the clinic

## Features in Detail

### Emergency Handling
- Emergency patients are automatically moved to the front of their assigned doctor's queue
- System maintains fairness for non-emergency cases while prioritizing emergencies

### Patient Assignment Logic
1. Attempts to match patient with specialist
2. For children under 16, prioritizes pediatricians
3. Falls back to general practitioners if no specialist is available
4. Places patients in unassigned queue if no suitable doctor is available

### Logging
- All transactions are logged to `clinic_transactions.log`
- Provides audit trail of all system activities
- Includes error messages and assignment details

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.
