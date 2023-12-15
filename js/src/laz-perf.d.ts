type Pointer = number

declare class LASZip {
  constructor()
  delete(): void

  open(data: Pointer, length: number): void
  getPoint(dest: Pointer): void
  getCount(): number
  getPointLength(): number
  getPointFormat(): number
}

declare class LASZipWriter {
  constructor()
  delete(): void

  open(b: Pointer, hb: Pointer, chunkSize: number): void
  close(): void
  writePoint(dest: Pointer): void
  getBytesWritten(): number
}

declare class ChunkDecoder {
  constructor()
  delete(): void

  open(
    pointDataRecordFormat: number,
    pointDataRecordLength: number,
    pointer: Pointer
  ): void

  getPoint(pointer: Pointer): void
}

export declare interface LazPerf extends EmscriptenModule {
  LASZip: typeof LASZip
  LASZipWriter: typeof LASZipWriter
  ChunkDecoder: typeof ChunkDecoder
}

declare const createLazPerf: EmscriptenModuleFactory<LazPerf>
export default createLazPerf
