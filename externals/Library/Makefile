all:
	@echo "Building:   Logging"
	@$(MAKE) -s -C Logging/
	@echo "Building:   Network"
	@$(MAKE) -s -C Network/
	@echo "Building:   Event"
	@$(MAKE) -s -C Event/
	@echo "Building:   M"
	@$(MAKE) -s -C M/
	@echo "Archiving:  Library.a"
	@mkdir -p .build
	@cd .build;$(AR) -x ../Logging/Logging.a
	@cd .build;$(AR) -x ../Network/Network.a
	@cd .build;$(AR) -x ../Event/Event.a
	@cd .build;$(AR) -x ../M/M.a
	@cd .build;$(AR) -r ../Library.a *.o 2>/dev/null
	@cd ..
	@rm -rf .build
	
clean:
	@echo "Cleaning:   Network"
	@$(MAKE) -s -C Network clean
	@echo "Cleaning:   Logging"	
	@$(MAKE) -s -C Logging clean
	@echo "Cleaning:   Event"
	@$(MAKE) -s -C Event clean
	@echo "Cleaning:   M"
	@$(MAKE) -s -C M clean
	@echo "Removing:   library.a"
	@rm -f Library.a
