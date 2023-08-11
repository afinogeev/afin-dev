clk5351 = {
    'output': 0,        # 0, 1, 2
    'freq': 1000000,    # Hz
    'phase': 0,         # %
    'enable': 1,        # 0, 1
    'invert': 0,        # 0, 1
    'idrive': 2         # 2, 4, 6, 8
    }

dds9833 = {
    'mode': 'sin',  # sin, triangle, square1, square2, off
    'freq': 10000,  # Hz
    'phase': 0,     # degree
    'gain': 100     # %
    }

if __name__ == '__main__':
    pass