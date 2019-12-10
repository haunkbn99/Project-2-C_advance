
    int i = 1;
    int k = 0;
    printf("START\n");
    while (fgets(temp, 1000, data) != NULL)
    {
        if (i % 2 == 0)
        {
            char diemBus[100];
            temp[strlen(temp) - 1] = '\0';
            token = strtok(temp, "-");
            trim(token);
            JRB node = jrb_find_str(bus.diemBus, token);
            if (node == NULL)
            {
                jrb_insert_str(bus.diemBus, strdup(token), new_jval_i(k));
                k++;
            }
            while (token != NULL)
            {
                token = strtok(NULL, "-");
                if (token != NULL)
                {
                    trim(token);
                    node = jrb_find_str(bus.diemBus, token);
                    if (node == NULL)
                    {
                        jrb_insert_str(bus.diemBus, strdup(token), new_jval_i(k));
                        k++;
                    }
                }
                else
                    break;
            }
        }
        i++;
