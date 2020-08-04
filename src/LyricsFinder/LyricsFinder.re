[@bs.val] external fetch: string => Js.Promise.t('a) = "fetch";


type lyrics =
  | NotLoadedLyrics
  | ErrorFetchingLyrics
  | LoadedLyrics(string);

[@react.component]
let make = () => {
  let (lyrics, setLyrics) = React.useState(() => NotLoadedLyrics);
  let(artistName, setArtistName) = React.useState(() => "");
  let(songName, setSongName) = React.useState(() => "");


  let onSubmit = (e: ReactEvent.Form.t): unit => {
    ReactEvent.Form.preventDefault(e);
    Js.Promise.(
      fetch("https://api.lyrics.ovh/v1/" ++ artistName ++ "/" ++ songName)
      |> then_(response => response##json())
      |> then_(jsonResponse => {
          if(jsonResponse##hasOwnProperty("lyrics")){
          setLyrics(_previousState => LoadedLyrics(jsonResponse##lyrics));
          }else{
            setLyrics(_previousState => ErrorFetchingLyrics);
          }
          Js.Promise.resolve();

      })
      |> catch(_err => {
          setLyrics(_previousState => ErrorFetchingLyrics);
           Js.Promise.resolve();
      })
      |> ignore
    );
  };

  <div>
  <form onSubmit>
  <input type_="text" id="artist" placeholder	="artist" name="artist"value=artistName onChange={
    e => {
      setArtistName(ReactEvent.Form.target(e)##value);
    }
  } />
  <input type_="text" id="song" placeholder	="song" name="song"value=songName onChange={
    e => {
      setSongName(ReactEvent.Form.target(e)##value);
    }
  } />
  <button type_="submit"> {React.string("submit")} </button>
  </form>

  {switch (lyrics) {
    | NotLoadedLyrics => React.string("Search for a song")
    | ErrorFetchingLyrics => React.string("Error Loading Lyrics!")
    | LoadedLyrics(lyricsText) =>
    Js.String.split("\n", lyricsText)
    ->Belt.Array.map((lyricLine) => {
      <div><label>{React.string(lyricLine)}</label></div>;
    
   })
   ->React.array
  }}
  </div>;
}
