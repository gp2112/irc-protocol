{ clangStdenv, ncurses }: clangStdenv.mkDerivation rec {
  pname = "irc-protocol";
  version = "0.1.0";

  buildInputs = [ ncurses ];
  src = ./.;
  makeFlags = [ "PREFIX=$(out)" ];
}
