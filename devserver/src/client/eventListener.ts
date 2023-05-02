import { EventData } from "./EventData.interface";
import { webServerThing } from "./main";
import { updateMarker } from "./mapStore";

export function listenEventSource() {
  if (!!window.EventSource) {
    var source = new EventSource(webServerThing + "/events");

    source.addEventListener(
      "open",
      function (e) {
        console.log("Events Connected");
      },
      false
    );

    source.addEventListener(
      "error",
      function (e: any) {
        if (e.target.readyState !== EventSource.OPEN) {
          console.log("Events Disconnected");
        }
      },
      false
    );

    source.addEventListener(
      "message",
      function (e: EventData) {
        // Parse data
        const data: [number, number] = JSON.parse(e.data);

        // Update marker
        updateMarker("default", data);
      },
      false
    );
  }
}
