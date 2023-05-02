import { LatLngTuple } from "leaflet";

// Create a map
const map = L.map("map").setView([51.5, -0.09], 15);
const markers: Map<string, L.Marker> = new Map(); // this is a regular JS Map btw.

// Create the map tiles
L.tileLayer("https://tile.openstreetmap.org/{z}/{x}/{y}.png", {
  maxZoom: 19,
  attribution:
    '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>',
}).addTo(map);

export function updateMarker(name: string, latLng: LatLngTuple) {
  let marker = markers.get(name);
  if (marker) {
    marker.setLatLng(latLng);
    map.setView(marker.getLatLng());
  } else {
    marker = L.marker(latLng).addTo(map);
    map.setView(marker.getLatLng());
    markers.set(name, marker);
  }
  return marker;
}
