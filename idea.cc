// iguana user code
clas12reader reader;
iguana::algorithm algo;
algo.Start();
auto bank3 = algo.CreateBank("new::bank");
auto read_action = [&algo, &bank3](clas12reader* CR) {
  algo.Run(
      *CR->bank1(),
      *CR->bank2(),
      bank3
      );
}
reader.SetReadAction(read_action);

// FIXME
// FIXME: handle hipo::bank::rowlist iteration
// FIXME
