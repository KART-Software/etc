export interface Transport {
  connect(): Promise<void>;
  disconnect(): Promise<void>;
  send(text: string): Promise<void>;
  isConnected(): boolean;
  setOnLineReceived(fn: (line: string) => void): void;
  setOnDisconnect(fn: () => void): void;
}
