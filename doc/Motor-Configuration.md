# Motor Stage Configuration

## 1. Current rating

TI DRV8434A uses VREF to set motor current:

```
VREF = current-limit x 1.32 V/A
```

Motor | Current Rating<br>(A) | VREF<br>(V)
--|--|--
Nanotec SCA2018S0604-B | 0.6 | 0.792
Nanotec LA201S06-B-UEAI | 0.6 | 0.792
Nanotec LGA281S10-B-UGAQ-019 | 1 | 1.32

## 2. Resolution

### 2.1 Beam stopper

- X-axis
  0.3048 mm/rev

- Z-axis
