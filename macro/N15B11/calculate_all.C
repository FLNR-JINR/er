void calculate_all()
{
    UInt_t numB11gas = 0;
    UInt_t numB11semi = 0;
    UInt_t numN15gas = 0;
    UInt_t numN15semi = 0;


    FILE* file = fopen("output_parallel/numbers.txt", "r");
    if (file == 0) {
        cerr << "numbers.txt was not found" << endl;
	return;
    } else {

        UInt_t iNum = 0;
        UInt_t iT = 0;
        while (fscanf(file, "%d", &iNum) > 0) {
            if (iT == 0) {
                numB11gas += iNum;
                iT++;
            }
            else if (iT == 1) {
                numB11semi += iNum;
                iT++;
            }
            else if (iT == 2) {
                numN15gas += iNum;
                iT++;
            }
            else if (iT == 3) {
                numN15semi += iNum;
                iT = 0;
            }

        }
        fclose(file);
    }
/*
    cout << "nB11Gas: " << numB11gas << "\t" << "nB11Semi: " << numB11semi << endl;
    cout << "nN15Gas: " << numN15gas << "\t" << "nN15Semi: " << numN15semi << endl;
*/
    cout << "nB11Gas: " << numB11gas << endl;
    cout << "nN15Gas: " << numN15gas << endl;
}
//-----------------------------------------------------------------------------------------------------------------
