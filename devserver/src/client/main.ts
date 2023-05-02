import { listenEventSource } from "./eventListener";
import { updateMarker } from "./mapStore";

export const webServerThing = "";

// Async function to load the initial map
fetch(webServerThing + "/data").then(async (res) => {
  const latLong = (await res.text()).split(",").map(Number.parseFloat);
  // ensure latlong contains two floats
  if (latLong.length !== 2) throw new Error("Invalid latlong");

  // this is to make typescript happy :)
  const marker = updateMarker("default", [latLong[0], latLong[1]]);
  marker.bindPopup("<b>Snacks stand</b><br>Status: Broken").openPopup();
});

listenEventSource();
