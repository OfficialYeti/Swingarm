# ADXL001 Module

	- Init - GPIO Pins config (How to bind GPIO (for ex. PE10 PE11) with ADC Chennel?)
		- Number of channels - for sensor ID counter
		- Caibration linear function - f(x) = ax + b
		- Threshold
		- Count of samples to save after threshold reached
	- IRQ Handler (Using component configuration)
		- Acquire sensor bit data
		- Pass data to proccessing methods
			- Calculate acceleration
			- Find threshold
			- Save set of samples